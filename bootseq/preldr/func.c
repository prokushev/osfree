/*
 *
 *
 */

#include <shared.h>
#include <filesys.h>

void (*disk_read_func) (int, int, int);
void (*disk_read_hook) (int, int, int);

int mem_lower = 639;
int mem_upper = 16384;

/* filesystem common variables */
int filepos;
int filemax;

/* disk buffer parameters */
int buf_drive = -1;
int buf_track;
struct geometry buf_geom;

grub_error_t errnum;
//int print_possibilities;

unsigned long saved_drive;
unsigned long saved_partition;

unsigned long current_drive;
unsigned long current_partition;
int           current_slice;
unsigned long part_start;
unsigned long part_length;
int           fsmax;

int debug = 0;
struct geometry buf_geom;

/* filesystem type */
int fsys_type = NUM_FSYS;
#ifndef NO_BLOCK_FILES
static int block_file = 0;
#endif /* NO_BLOCK_FILES */

static inline unsigned long
log2 (unsigned long word);

/* FIXME: BSD evil hack */
#include "freebsd.h"
int bsd_evil_hack;

struct fsys_entry fsys_table[NUM_FSYS + 1] =
{
  /* TFTP should come first because others don't handle net device.  */
# ifdef FSYS_TFTP
  {"tftp", tftp_mount, tftp_read, tftp_dir, tftp_close, 0},
# endif
# ifdef FSYS_FAT
  {"fat", fat_mount, fat_read, fat_dir, 0, 0},
# endif
# ifdef FSYS_EXT2FS
  {"ext2fs", ext2fs_mount, ext2fs_read, ext2fs_dir, 0, 0},
# endif
# ifdef FSYS_MINIX
  {"minix", minix_mount, minix_read, minix_dir, 0, 0},
# endif
# ifdef FSYS_REISERFS
  {"reiserfs", reiserfs_mount, reiserfs_read, reiserfs_dir, 0, reiserfs_embed},
# endif
# ifdef FSYS_VSTAFS
  {"vstafs", vstafs_mount, vstafs_read, vstafs_dir, 0, 0},
# endif
# ifdef FSYS_JFS
  {"jfs", jfs_mount, jfs_read, jfs_dir, 0, jfs_embed},
# endif
# ifdef FSYS_XFS
  {"xfs", xfs_mount, xfs_read, xfs_dir, 0, 0},
# endif
# ifdef FSYS_UFS2
  {"ufs2", ufs2_mount, ufs2_read, ufs2_dir, 0, ufs2_embed},
# endif
# ifdef FSYS_ISO9660
  {"iso9660", iso9660_mount, iso9660_read, iso9660_dir, 0, 0},
# endif
  /* XX FFS should come last as it's superblock is commonly crossing tracks
     on floppies from track 1 to 2, while others only use 1.  */
# ifdef FSYS_FFS
  {"ffs", ffs_mount, ffs_read, ffs_dir, 0, ffs_embed},
# endif
  {0, 0, 0, 0, 0, 0}
};

/* These are defined in asm.S, and never be used elsewhere, so declare the
   prototypes here.  */
extern int biosdisk_int13_extensions (int ax, int drive, void *dap);
extern int biosdisk_standard (int ah, int drive,
                              int coff, int hoff, int soff,
                              int nsec, int segment);
extern int check_int13_extensions (int drive);
extern int get_diskinfo_standard (int drive,
                                  unsigned long *cylinders,
                                  unsigned long *heads,
                                  unsigned long *sectors);

int
rawread (int drive, int sector, int byte_offset, int byte_len, char *buf)
{
  int slen, sectors_per_vtrack;
  int sector_size_bits = log2 (buf_geom.sector_size);

  if (byte_len <= 0)
    return 1;

  while (byte_len > 0 && !errnum)
    {
      int soff, num_sect, track, size = byte_len;
      char *bufaddr;

      /*
       *  Check track buffer.  If it isn't valid or it is from the
       *  wrong disk, then reset the disk geometry.
       */
      if (buf_drive != drive)
        {
          if (get_diskinfo (drive, &buf_geom))
            {
              errnum = ERR_NO_DISK;
              return 0;
            }
          buf_drive = drive;
          buf_track = -1;
          sector_size_bits = log2 (buf_geom.sector_size);
        }

      /* Make sure that SECTOR is valid.  */
      if (sector < 0 || sector >= buf_geom.total_sectors)
        {
          errnum = ERR_GEOM;
          return 0;
        }

      slen = ((byte_offset + byte_len + buf_geom.sector_size - 1)
              >> sector_size_bits);

      /* Eliminate a buffer overflow.  */
      if ((buf_geom.sectors << sector_size_bits) > BUFFERLEN)
        sectors_per_vtrack = (BUFFERLEN >> sector_size_bits);
      else
        sectors_per_vtrack = buf_geom.sectors;

      /* Get the first sector of track.  */
      soff = sector % sectors_per_vtrack;
      track = sector - soff;
      num_sect = sectors_per_vtrack - soff;
      bufaddr = ((char *) BUFFERADDR
                 + (soff << sector_size_bits) + byte_offset);

      if (track != buf_track)
        {
          int bios_err, read_start = track, read_len = sectors_per_vtrack;

          /*
           *  If there's more than one read in this entire loop, then
           *  only make the earlier reads for the portion needed.  This
           *  saves filling the buffer with data that won't be used!
           */
          if (slen > num_sect)
            {
              read_start = sector;
              read_len = num_sect;
              bufaddr = (char *) BUFFERADDR + byte_offset;
            }

          bios_err = biosdisk (BIOSDISK_READ, drive, &buf_geom,
                               read_start, read_len, BUFFERSEG);
          if (bios_err)
            {
              buf_track = -1;

              if (bios_err == BIOSDISK_ERROR_GEOMETRY)
                errnum = ERR_GEOM;
              else
                {
                  /*
                   *  If there was an error, try to load only the
                   *  required sector(s) rather than failing completely.
                   */
                  if (slen > num_sect
                      || biosdisk (BIOSDISK_READ, drive, &buf_geom,
                                   sector, slen, BUFFERSEG))
                    errnum = ERR_READ;

                  bufaddr = (char *) BUFFERADDR + byte_offset;
                }
            }
          else
            buf_track = track;

          if ((buf_track == 0 || sector == 0)
              && (PC_SLICE_TYPE (BUFFERADDR, 0) == PC_SLICE_TYPE_EZD
                  || PC_SLICE_TYPE (BUFFERADDR, 1) == PC_SLICE_TYPE_EZD
                  || PC_SLICE_TYPE (BUFFERADDR, 2) == PC_SLICE_TYPE_EZD
                  || PC_SLICE_TYPE (BUFFERADDR, 3) == PC_SLICE_TYPE_EZD))
            {
              /* This is a EZD disk map sector 0 to sector 1 */
              if (buf_track == 0 || slen >= 2)
                {
                  /* We already read the sector 1, copy it to sector 0 */
                  memmove ((char *) BUFFERADDR,
                           (char *) BUFFERADDR + buf_geom.sector_size,
                           buf_geom.sector_size);
                }
              else
                {
                  if (biosdisk (BIOSDISK_READ, drive, &buf_geom,
                                1, 1, BUFFERSEG))
                    errnum = ERR_READ;
                }
            }
        }

      if (size > ((num_sect << sector_size_bits) - byte_offset))
        size = (num_sect << sector_size_bits) - byte_offset;

      /*
       *  Instrumentation to tell which sectors were read and used.
       */
      if (disk_read_func)
        {
          int sector_num = sector;
          int length = buf_geom.sector_size - byte_offset;
          if (length > size)
            length = size;
          (*disk_read_func) (sector_num++, byte_offset, length);
          length = size - length;
          if (length > 0)
            {
              while (length > buf_geom.sector_size)
                {
                  (*disk_read_func) (sector_num++, 0, buf_geom.sector_size);
                  length -= buf_geom.sector_size;
                }
              (*disk_read_func) (sector_num, 0, length);
            }
        }

      grub_memmove (buf, bufaddr, size);

      buf += size;
      byte_len -= size;
      sector += num_sect;
      byte_offset = 0;
    }

  return (!errnum);
}


int
safe_parse_maxint (char **str_ptr, int *myint_ptr)
{
  char *ptr = *str_ptr;
  int myint = 0;
  int mult = 10, found = 0;

  /*
   *  Is this a hex number?
   */
  if (*ptr == '0' && tolower (*(ptr + 1)) == 'x')
    {
      ptr += 2;
      mult = 16;
    }

  while (1)
    {
      /* A bit tricky. This below makes use of the equivalence:
         (A >= B && A <= C) <=> ((A - B) <= (C - B))
         when C > B and A is unsigned.  */
      unsigned int digit;

      digit = tolower (*ptr) - '0';
      if (digit > 9)
        {
          digit -= 'a' - '0';
          if (mult == 10 || digit > 5)
            break;
          digit += 10;
        }

      found = 1;
      if (myint > ((MAXINT - digit) / mult))
        {
          errnum = ERR_NUMBER_OVERFLOW;
          return 0;
        }
      myint = (myint * mult) + digit;
      ptr++;
    }

  if (!found)
    {
      errnum = ERR_NUMBER_PARSING;
      return 0;
    }

  *str_ptr = ptr;
  *myint_ptr = myint;

  return 1;
}


static void
attempt_mount (void)
{
#ifndef STAGE1_5
  for (fsys_type = 0; fsys_type < NUM_FSYS; fsys_type++)
    if ((fsys_table[fsys_type].mount_func) ())
      break;

  if (fsys_type == NUM_FSYS && errnum == ERR_NONE)
    errnum = ERR_FSYS_MOUNT;
#else
  fsys_type = 0;
  if ((*(fsys_table[fsys_type].mount_func)) () != 1)
    {
      fsys_type = NUM_FSYS;
      errnum = ERR_FSYS_MOUNT;
    }
#endif
}


/* Forward declarations.  */
int next_bsd_partition (unsigned long drive, unsigned long dest,
                        unsigned long *partition, int *type,
                        unsigned long *start, unsigned long *len,
                        unsigned long *offset, int *entry,
                        unsigned long *ext_offset, char *buf);

int next_pc_slice (unsigned long drive, unsigned long dest,
                   unsigned long *partition, int *type,
                   unsigned long *start, unsigned long *len,
                   unsigned long *offset, int *entry,
                   unsigned long *ext_offset, char *buf);


/* Get next BSD partition in current PC slice.  */
int next_bsd_partition (unsigned long drive, unsigned long dest,
                        unsigned long *partition, int *type,
                        unsigned long *start, unsigned long *len,
                        unsigned long *offset, int *entry,
                        unsigned long *ext_offset, char *buf)
{
      int i;
      int bsd_part_no = (*partition & 0xFF00) >> 8;

      /* If this is the first time...  */
      if (bsd_part_no == 0xFF)
        {
          /* Check if the BSD label is within current PC slice.  */
          if (*len < BSD_LABEL_SECTOR + 1)
            {
              errnum = ERR_BAD_PART_TABLE;
              return 0;
            }

          /* Read the BSD label.  */
          if (! rawread (drive, *start + BSD_LABEL_SECTOR,
                         0, SECTOR_SIZE, buf))
            return 0;

          /* Check if it is valid.  */
          if (! BSD_LABEL_CHECK_MAG (buf))
            {
              errnum = ERR_BAD_PART_TABLE;
              return 0;
            }

          bsd_part_no = -1;
        }

      /* Search next valid BSD partition.  */
      for (i = bsd_part_no + 1; i < BSD_LABEL_NPARTS (buf); i++)
        {
          if (BSD_PART_TYPE (buf, i))
            {
              /* Note that *TYPE and *PARTITION were set
                 for current PC slice.  */
              *type = (BSD_PART_TYPE (buf, i) << 8) | (*type & 0xFF);
              *start = BSD_PART_START (buf, i);
              *len = BSD_PART_LENGTH (buf, i);
              *partition = (*partition & 0xFF00FF) | (i << 8);

#ifndef STAGE1_5
              /* XXX */
              if ((drive & 0x80) && BSD_LABEL_DTYPE (buf) == DTYPE_SCSI)
                bsd_evil_hack = 4;
#endif /* ! STAGE1_5 */

              return 1;
            }
        }

      errnum = ERR_NO_PART;
      return 0;
}


/* Get next PC slice. Be careful of that this function may return
   an empty PC slice (i.e. a partition whose type is zero) as well.  */
int next_pc_slice (unsigned long drive, unsigned long dest,
                     unsigned long *partition, int *type,
                     unsigned long *start, unsigned long *len,
                     unsigned long *offset, int *entry,
                     unsigned long *ext_offset, char *buf)
{
      int pc_slice_no = (*partition & 0xFF0000) >> 16;

      /* If this is the first time...  */
      if (pc_slice_no == 0xFF)
        {
          *offset = 0;
          *ext_offset = 0;
          *entry = -1;
          pc_slice_no = -1;
        }

      /* Read the MBR or the boot sector of the extended partition.  */
      if (! rawread (drive, *offset, 0, SECTOR_SIZE, buf))
        return 0;

      /* Check if it is valid.  */
      if (! PC_MBR_CHECK_SIG (buf))
        {
          errnum = ERR_BAD_PART_TABLE;
          return 0;
        }

      /* Increase the entry number.  */
      (*entry)++;

      /* If this is out of current partition table...  */
      if (*entry == PC_SLICE_MAX)
        {
          int i;

          /* Search the first extended partition in current table.  */
          for (i = 0; i < PC_SLICE_MAX; i++)
            {
              if (IS_PC_SLICE_TYPE_EXTENDED (PC_SLICE_TYPE (buf, i)))
                {
                  /* Found. Set the new offset and the entry number,
                     and restart this function.  */
                  *offset = *ext_offset + PC_SLICE_START (buf, i);
                  if (! *ext_offset)
                    *ext_offset = *offset;
                  *entry = -1;
                  return next_pc_slice (drive, dest,
                                        partition, type,
                                        start, len,
                                        offset, entry,
                                        ext_offset, buf);
                }
            }

          errnum = ERR_NO_PART;
          return 0;
        }

      *type = PC_SLICE_TYPE (buf, *entry);
      *start = *offset + PC_SLICE_START (buf, *entry);
      *len = PC_SLICE_LENGTH (buf, *entry);

      /* The calculation of a PC slice number is complicated, because of
         the rather odd definition of extended partitions. Even worse,
         there is no guarantee that this is consistent with every
         operating systems. Uggh.  */
      if (pc_slice_no < PC_SLICE_MAX
          || (! IS_PC_SLICE_TYPE_EXTENDED (*type)
              && *type != PC_SLICE_TYPE_NONE))
        pc_slice_no++;

      *partition = (pc_slice_no << 16) | 0xFFFF;
      return 1;
}


/* Get the information on next partition on the drive DRIVE.
   The caller must not modify the contents of the arguments when
   iterating this function. The partition representation in GRUB will
   be stored in *PARTITION. Likewise, the partition type in *TYPE, the
   start sector in *START, the length in *LEN, the offset of the
   partition table in *OFFSET, the entry number in the table in *ENTRY,
   the offset of the extended partition in *EXT_OFFSET.
   BUF is used to store a MBR, the boot sector of a partition, or
   a BSD label sector, and it must be at least 512 bytes length.
   When calling this function first, *PARTITION must be initialized to
   0xFFFFFF. The return value is zero if fails, otherwise non-zero.  */
int
next_partition (unsigned long drive, unsigned long dest,
                unsigned long *partition, int *type,
                unsigned long *start, unsigned long *len,
                unsigned long *offset, int *entry,
                unsigned long *ext_offset, char *buf)
{
  /* Start the body of this function.  */

#ifndef STAGE1_5
  if (current_drive == NETWORK_DRIVE)
    return 0;
#endif

  /* If previous partition is a BSD partition or a PC slice which
     contains BSD partitions...  */
  if ((*partition != 0xFFFFFF && IS_PC_SLICE_TYPE_BSD (*type & 0xff))
      || ! (drive & 0x80))
    {
      if (*type == PC_SLICE_TYPE_NONE)
        *type = PC_SLICE_TYPE_FREEBSD;

      /* Get next BSD partition, if any.  */
      if (next_bsd_partition (drive, dest,
                              partition, type,
                              start, len,
                              offset, entry,
                              ext_offset, buf))
        return 1;

      /* If the destination partition is a BSD partition and current
         BSD partition has any error, abort the operation.  */
      if ((dest & 0xFF00) != 0xFF00
          && ((dest & 0xFF0000) == 0xFF0000
              || (dest & 0xFF0000) == (*partition & 0xFF0000)))
        return 0;

      /* Ignore the error.  */
      errnum = ERR_NONE;
    }

  return next_pc_slice (drive, dest,
                        partition, type,
                        start, len,
                        offset, entry,
                        ext_offset, buf);
}


/* For simplicity.  */
//auto int next (void);
int next (int *bsd_part, int *pc_slice,
          unsigned long current_drive, unsigned long dest_partition,
          unsigned long *current_partition, int *current_slice,
          unsigned long *part_start, unsigned long *part_length,
          unsigned long *part_offset, int *entry, unsigned long *ext_offset, char *buf)
{
    int ret = next_partition (current_drive, dest_partition,
                              current_partition, current_slice,
                              part_start, part_length,
                              part_offset, entry, ext_offset, buf);
    *bsd_part = (*current_partition >> 8) & 0xFF;
    *pc_slice = *current_partition >> 16;
    return ret;
}


/* Open a partition.  */
int
real_open_partition (int flags)
{
  unsigned long dest_partition = current_partition;
  unsigned long part_offset;
  unsigned long ext_offset;
  int entry;
  char buf[SECTOR_SIZE];
  int bsd_part, pc_slice;

#ifndef STAGE1_5
  /* network drive */
  if (current_drive == NETWORK_DRIVE)
    return 1;

  if (! sane_partition ())
    return 0;
#endif

  bsd_evil_hack = 0;
  current_slice = 0;
  part_start = 0;

  /* Make sure that buf_geom is valid. */
  if (buf_drive != current_drive)
    {
      if (get_diskinfo (current_drive, &buf_geom))
        {
          errnum = ERR_NO_DISK;
          return 0;
        }
      buf_drive = current_drive;
      buf_track = -1;
    }
  part_length = buf_geom.total_sectors;

  /* If this is the whole disk, return here.  */
  if (! flags && current_partition == 0xFFFFFF)
    return 1;

  if (flags)
    dest_partition = 0xFFFFFF;

  /* Initialize CURRENT_PARTITION for next_partition.  */
  current_partition = 0xFFFFFF;

  while (next (&bsd_part, &pc_slice,
               current_drive, dest_partition,
               &current_partition, &current_slice,
               &part_start, &part_length,
               &part_offset, &entry, &ext_offset, buf))
    {
#ifndef STAGE1_5
    loop_start:

      cur_part_offset = part_offset;
      cur_part_addr = BOOT_PART_TABLE + (entry << 4);
#endif /* ! STAGE1_5 */

      /* If this is a valid partition...  */
      if (current_slice)
        {
#ifndef STAGE1_5
          /* Display partition information.  */
          if (flags && ! IS_PC_SLICE_TYPE_EXTENDED (current_slice))
            {
              if (! do_completion)
                {
                  if (current_drive & 0x80)
                    grub_printf ("   Partition num: %d, ",
                                 current_partition >> 16);

                  if (! IS_PC_SLICE_TYPE_BSD (current_slice))
                    check_and_print_mount (flags);
                  else
                    {
                      int got_part = 0;
                      int saved_slice = current_slice;

                      while (next (&bsd_part, &pc_slice,
                                   current_drive, dest_partition,
                                   &current_partition, &current_slice,
                                   &part_start, &part_length,
                                   &part_offset, &entry, &ext_offset, buf))
                        {
                          if (bsd_part == 0xFF)
                            break;

                          if (! got_part)
                            {
                              grub_printf ("[BSD sub-partitions immediately follow]\n");
                              got_part = 1;
                            }

                          grub_printf ("     BSD Partition num: \'%c\', ",
                                       bsd_part + 'a');
                          check_and_print_mount (flags);
                        }

                      if (! got_part)
                        grub_printf (" No BSD sub-partition found, partition type 0x%x\n",
                                     saved_slice);

                      if (errnum)
                        {
                          errnum = ERR_NONE;
                          break;
                        }

                      goto loop_start;
                    }
                }
              else
                {
                  if (bsd_part != 0xFF)
                    {
                      char str[16];

                      if (! (current_drive & 0x80)
                          || (dest_partition >> 16) == pc_slice)
                        grub_sprintf (str, "%c)", bsd_part + 'a');
                      else
                        grub_sprintf (str, "%d,%c)",
                                      pc_slice, bsd_part + 'a');
                      print_a_completion (str);
                    }
                  else if (! IS_PC_SLICE_TYPE_BSD (current_slice))
                    {
                      char str[8];

                      grub_sprintf (str, "%d)", pc_slice);
                      print_a_completion (str);
                    }
                }
            }

          errnum = ERR_NONE;
#endif /* ! STAGE1_5 */

          /* Check if this is the destination partition.  */
          if (! flags
              && (dest_partition == current_partition
                  || ((dest_partition >> 16) == 0xFF
                      && ((dest_partition >> 8) & 0xFF) == bsd_part)))
            return 1;
        }
    }

#ifndef STAGE1_5
  if (flags)
    {
      errnum = ERR_NONE;
      if (! (current_drive & 0x80))
        {
          current_partition = 0xFFFFFF;
          current_slice = 0;
          part_start = 0;
          part_length = buf_geom.total_sectors;
          check_and_print_mount (flags);
        }
      else
        {
          cur_part_addr = 0;
          cur_part_offset = 0;
        }

      errnum = ERR_NONE;
      return 1;
    }
#endif /* ! STAGE1_5 */

  return 0;
}


int
open_partition (void)
{
  return real_open_partition (0);
}


/*
 *  This performs a "mount" on the current device, both drive and partition
 *  number.
 */

int
open_device (void)
{
  if (open_partition ())
    attempt_mount ();

  if (errnum != ERR_NONE)
    return 0;

  return 1;
}


char *
set_device (char *device)
{
#ifdef STAGE1_5
    /* In Stage 1.5, the first 4 bytes of FILENAME has a device number.  */
  unsigned long dev = *((unsigned long *) device);
  int drive = (dev >> 24) & 0xFF;
  int partition = dev & 0xFFFFFF;

  /* If DRIVE is disabled, use SAVED_DRIVE instead.  */
  if (drive == GRUB_INVALID_DRIVE)
    current_drive = saved_drive;
  else
    current_drive = drive;

  /* The `partition' part must always have a valid number.  */
  current_partition = partition;

  return device + sizeof (unsigned long);

#else /* ! STAGE1_5 */

  int result = 0;

  incomplete = 0;
  disk_choice = 1;
  part_choice = PART_UNSPECIFIED;
  current_drive = saved_drive;
  current_partition = 0xFFFFFF;

  if (*device == '(' && !*(device + 1))
    /* user has given '(' only, let disk_choice handle what disks we have */
    return device + 1;

  if (*device == '(' && *(++device))
    {
      if (*device != ',' && *device != ')')
        {
          char ch = *device;
#ifdef SUPPORT_NETBOOT
          if (*device == 'n' && !network_ready)
            bootp();

          if (*device == 'f' || *device == 'h'
              || (*device == 'n' && network_ready)
              || (*device == 'c' && cdrom_drive != GRUB_INVALID_DRIVE))
#else
          if (*device == 'f' || *device == 'h'
              || (*device == 'c' && cdrom_drive != GRUB_INVALID_DRIVE))
#endif /* SUPPORT_NETBOOT */
            {
              /* user has given '([fhn]', check for resp. add 'd' and
                 let disk_choice handle what disks we have */
              if (!*(device + 1))
                {
                  device++;
                  *device++ = 'd';
                  *device = '\0';
                  return device;
                }
              else if (*(device + 1) == 'd' && !*(device + 2))
                return device + 2;
            }

          if ((*device == 'f'
               || *device == 'h'
#ifdef SUPPORT_NETBOOT
               || (*device == 'n' && network_ready)
#endif
               || (*device == 'c' && cdrom_drive != GRUB_INVALID_DRIVE))
              && (device += 2, (*(device - 1) != 'd')))
            errnum = ERR_NUMBER_PARSING;

#ifdef SUPPORT_NETBOOT
          if (ch == 'n' && network_ready)
            current_drive = NETWORK_DRIVE;
          else
#endif /* SUPPORT_NETBOOT */
            {
              if (ch == 'c' && cdrom_drive != GRUB_INVALID_DRIVE)
                current_drive = cdrom_drive;
              else
                {
                  safe_parse_maxint (&device, (int *) &current_drive);

                  disk_choice = 0;
                  if (ch == 'h')
                    current_drive += 0x80;
                }
            }
        }

      if (errnum)
        return 0;

      if (*device == ')')
        {
          part_choice = PART_CHOSEN;
          result = 1;
        }
      else if (*device == ',')
        {
          /* Either an absolute PC or BSD partition. */
          disk_choice = 0;
          part_choice ++;
          device++;

          if (*device >= '0' && *device <= '9')
            {
              part_choice ++;
              current_partition = 0;

              if (!(current_drive & 0x80)
                  || !safe_parse_maxint (&device, (int *) &current_partition)
                  || current_partition > 254)
                {
                  errnum = ERR_DEV_FORMAT;
                  return 0;
                }

              current_partition = (current_partition << 16) + 0xFFFF;

              if (*device == ',')
                device++;

              if (*device >= 'a' && *device <= 'h')
                {
                  current_partition = (((*(device++) - 'a') << 8)
                                       | (current_partition & 0xFF00FF));
                }
            }
          else if (*device >= 'a' && *device <= 'h')
            {
              part_choice ++;
              current_partition = ((*(device++) - 'a') << 8) | 0xFF00FF;
            }

          if (*device == ')')
            {
              if (part_choice == PART_DISK)
                {
                  current_partition = saved_partition;
                  part_choice ++;
                }

              result = 1;
            }
        }
    }

  if (! sane_partition ())
    return 0;

  if (result)
    return device + 1;
  else
    {
      if (!*device)
        incomplete = 1;
      errnum = ERR_DEV_FORMAT;
    }

  return 0;

#endif /* ! STAGE1_5 */
}



static char *
setup_part (char *filename)
{
#ifdef STAGE1_5

  if (! (filename = set_device (filename)))
    {
      current_drive = GRUB_INVALID_DRIVE;
      return 0;
    }

# ifndef NO_BLOCK_FILES
  if (*filename != '/')
    open_partition ();
  else
# endif /* ! NO_BLOCK_FILES */
    open_device ();

#else /* ! STAGE1_5 */

  if (*filename == '(')
    {
      if ((filename = set_device (filename)) == 0)
        {
          current_drive = GRUB_INVALID_DRIVE;
          return 0;
        }
# ifndef NO_BLOCK_FILES
      if (*filename != '/')
        open_partition ();
      else
# endif /* ! NO_BLOCK_FILES */
        open_device ();
    }
  else if (saved_drive != current_drive
           || saved_partition != current_partition
           || (*filename == '/' && fsys_type == NUM_FSYS)
           || buf_drive == -1)
    {
      current_drive = saved_drive;
      current_partition = saved_partition;
      /* allow for the error case of "no filesystem" after the partition
         is found.  This makes block files work fine on no filesystem */
# ifndef NO_BLOCK_FILES
      if (*filename != '/')
        open_partition ();
      else
# endif /* ! NO_BLOCK_FILES */
        open_device ();
    }

#endif /* ! STAGE1_5 */

  if (errnum && (*filename == '/' || errnum != ERR_FSYS_MOUNT))
    return 0;
  else
    errnum = 0;

#ifndef STAGE1_5
  if (!sane_partition ())
    return 0;
#endif

  return filename;
}


int
devread (int sector, int byte_offset, int byte_len, char *buf)
{
  /*
   *  Check partition boundaries
   */
  if (sector < 0
      || ((sector + ((byte_offset + byte_len - 1) >> SECTOR_BITS))
          >= part_length))
    {
      errnum = ERR_OUTSIDE_PART;
      return 0;
    }

  /*
   *  Get the read to the beginning of a partition.
   */
  sector += byte_offset >> SECTOR_BITS;
  byte_offset &= SECTOR_SIZE - 1;

#if !defined(STAGE1_5)
  if (disk_read_hook && debug)
    printf ("<%d, %d, %d>", sector, byte_offset, byte_len);
#endif /* !STAGE1_5 */

  /*
   *  Call RAWREAD, which is very similar, but:
   *
   *    --  It takes an extra parameter, the drive number.
   *    --  It requires that "sector" is relative to the beginning
   *            of the disk.
   *    --  It doesn't handle offsets of more than 511 bytes into the
   *            sector.
   */
  return rawread (current_drive, part_start + sector, byte_offset,
                  byte_len, buf);
}


int
substring (const char *s1, const char *s2)
{
  while (*s1 == *s2)
    {
      /* The strings match exactly. */
      if (! *(s1++))
        return 0;
      s2 ++;
    }

  /* S1 is a substring of S2. */
  if (*s1 == 0)
    return -1;

  /* S1 isn't a substring. */
  return 1;
}


int
grub_tolower (int c)
{
  if (c >= 'A' && c <= 'Z')
    return (c + ('a' - 'A'));

  return c;
}


//#if defined(FSYS_ISO9660)
int
grub_memcmp (const char *s1, const char *s2, int n)
{
  while (n)
    {
      if (*s1 < *s2)
        return -1;
      else if (*s1 > *s2)
        return 1;
      s1++;
      s2++;
      n--;
    }

  return 0;
}
//#endif /* ! FSYS_ISO9660 */


long
grub_memcheck (unsigned long addr, long len)
{
    // Physical address:
    if ( (addr < RAW_ADDR (0x1000))
        || ((addr <  RAW_ADDR (0x100000)) && (RAW_ADDR(mem_lower * 1024) < (addr + len)))
        || ((addr >= RAW_ADDR (0x100000)) && (RAW_ADDR(mem_upper * 1024) < ((addr - 0x100000) + len))) )
    {
        errnum = ERR_WONT_FIT;
        //printk("freeldr_memcheck: ERR_WONT_FIT");
        //printk("freeldr_memcheck(): addr = 0x%08lx, len = %u", addr, len);
        return 0;
    }

    return 1;
}


void *
grub_memmove (void *_to, const void *_from, int _len)
{

    char *from = (char *)_from;
    char *to   = _to;

    if (grub_memcheck ((unsigned long)(_to), _len))
    {
        if ( from == to )
        {
            return( to );
        }
        if ( from < to  &&  from + _len > to )
        {
            to += _len;
            from += _len;
            while( _len != 0 )
            {
                *--to = *--from;
                _len--;
            }
        }
        else
        {
            while( _len != 0 )
            {
                *to++ = *from++;
                _len--;
            }
        }
    }

    return( to );
}


void *
grub_memset (void *start, int c, int len)
{
  char *p = start;

  if (grub_memcheck ((int) start, len))
    {
      while (len -- > 0)
        *p ++ = c;
    }

  return errnum ? NULL : start;
}


int
grub_isspace (int c)
{
  switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      return 1;
    default:
      break;
    }

  return 0;
}


static inline unsigned long
log2 (unsigned long word)
{
  //asm volatile ("bsfl %1,%0"
  //              : "=r" (word)
  //              : "r" (word));
  unsigned long l;

  l = word;

  __asm {
    mov  eax, l
    bsf  eax, eax
    mov  l,   eax
  }

  return l;
}


/*
 *  This is the generic file open function.
 */

int
grub_open (char *filename)
{
#ifndef STAGE1_5
  const int buf_size = 1500;
  const char *try_filenames[] = { "menu.lst", "m" };
  char fn[buf_size]; /* arbitrary... */
  char *filename_orig = filename;
  int trycount = 0;

  if (grub_strlen(filename) > buf_size)
    {
      printf("Buffer overflow: %s(%d)\n", __FILE__, __LINE__);
      while (1) {}
    }

  /* initially, we need to copy filename to fn */
  grub_strcpy(fn, filename_orig);
  fn[grub_strlen(filename_orig)] = 0;

restart:
  errnum = 0; /* hrm... */
  filename = fn;
#endif

#ifndef NO_DECOMPRESSION
  compressed_file = 0;
#endif /* NO_DECOMPRESSION */

  /* if any "dir" function uses/sets filepos, it must
     set it to zero before returning if opening a file! */
  filepos = 0;

  if (!(filename = (char *)setup_part (filename)))
    {
      if (errnum == ERR_FILE_NOT_FOUND)
        goto retry;
      else
        return 0;
    }

#ifndef NO_BLOCK_FILES
  block_file = 0;
#endif /* NO_BLOCK_FILES */

  /* This accounts for partial filesystem implementations. */
  fsmax = MAXINT;

  if (*filename != '/')
    {
#ifndef NO_BLOCK_FILES
      char *ptr = filename;
      int tmp, list_addr = BLK_BLKLIST_START;
      filemax = 0;

      while (list_addr < BLK_MAX_ADDR)
        {
          tmp = 0;
          safe_parse_maxint (&ptr, &tmp);
          errnum = 0;

          if (*ptr != '+')
            {
              if ((*ptr && *ptr != '/' && !isspace (*ptr))
                  || tmp == 0 || tmp > filemax)
                errnum = ERR_BAD_FILENAME;
              else
                filemax = tmp;

              break;
            }

          /* since we use the same filesystem buffer, mark it to
             be remounted */
          fsys_type = NUM_FSYS;

          BLK_BLKSTART (list_addr) = tmp;
          ptr++;

          if (!safe_parse_maxint (&ptr, &tmp)
              || tmp == 0
              || (*ptr && *ptr != ',' && *ptr != '/' && !isspace (*ptr)))
            {
              errnum = ERR_BAD_FILENAME;
              break;
            }

          BLK_BLKLENGTH (list_addr) = tmp;

          filemax += (tmp * SECTOR_SIZE);
          list_addr += BLK_BLKLIST_INC_VAL;

          if (*ptr != ',')
            break;

          ptr++;
        }

      if (list_addr < BLK_MAX_ADDR && ptr != filename && !errnum)
        {
          block_file = 1;
          BLK_CUR_FILEPOS = 0;
          BLK_CUR_BLKLIST = BLK_BLKLIST_START;
          BLK_CUR_BLKNUM = 0;

#ifndef NO_DECOMPRESSION
          return gunzip_test_header ();
#else /* NO_DECOMPRESSION */
          return 1;
#endif /* NO_DECOMPRESSION */
        }
#else /* NO_BLOCK_FILES */
      errnum = ERR_BAD_FILENAME;
#endif /* NO_BLOCK_FILES */
    }

  if (!errnum && fsys_type == NUM_FSYS)
    errnum = ERR_FSYS_MOUNT;

# ifndef STAGE1_5
  /* set "dir" function to open a file */
  print_possibilities = 0;
# endif

  if (!errnum && (*(fsys_table[fsys_type].dir_func)) (filename))
    {
#ifndef NO_DECOMPRESSION
      return gunzip_test_header ();
#else /* NO_DECOMPRESSION */
      return 1;
#endif /* NO_DECOMPRESSION */
    }

retry:
# ifndef STAGE1_5
  if (trycount < sizeof(try_filenames) / sizeof(*try_filenames))
    {
      unsigned int l = grub_strlen(filename_orig);
      unsigned int ll = grub_strlen(try_filenames[trycount]);
      int j;

      grub_memmove(fn, filename_orig, l);

      if (filename_orig[l - 1] != '/')
        fn[l++] = '/';
      for (j = 0; j < ll; j++)
        fn[l + j] = try_filenames[trycount][j];
      fn[l + ll] = 0;

      grub_printf("Previous try failed, trying \"%s\"\n", fn);
      trycount++;
      goto restart;
    }
# endif


  return 0;
}


int
grub_read (char *buf, int len)
{
  /* Make sure "filepos" is a sane value */
  if ((filepos < 0) || (filepos > filemax))
    filepos = filemax;

  /* Make sure "len" is a sane value */
  if ((len < 0) || (len > (filemax - filepos)))
    len = filemax - filepos;

  /* if target file position is past the end of
     the supported/configured filesize, then
     there is an error */
  if (filepos + len > fsmax)
    {
      errnum = ERR_FILELENGTH;
      return 0;
    }

#ifndef NO_DECOMPRESSION
  if (compressed_file)
    return gunzip_read (buf, len);
#endif /* NO_DECOMPRESSION */

#ifndef NO_BLOCK_FILES
  if (block_file)
    {
      int size, off, ret = 0;

      while (len && !errnum)
        {
          /* we may need to look for the right block in the list(s) */
          if (filepos < BLK_CUR_FILEPOS)
            {
              BLK_CUR_FILEPOS = 0;
              BLK_CUR_BLKLIST = BLK_BLKLIST_START;
              BLK_CUR_BLKNUM = 0;
            }

          /* run BLK_CUR_FILEPOS up to filepos */
          while (filepos > BLK_CUR_FILEPOS)
            {
              if ((filepos - (BLK_CUR_FILEPOS & ~(SECTOR_SIZE - 1)))
                  >= SECTOR_SIZE)
                {
                  BLK_CUR_FILEPOS += SECTOR_SIZE;
                  BLK_CUR_BLKNUM++;

                  if (BLK_CUR_BLKNUM >= BLK_BLKLENGTH (BLK_CUR_BLKLIST))
                    {
                      BLK_CUR_BLKLIST += BLK_BLKLIST_INC_VAL;
                      BLK_CUR_BLKNUM = 0;
                    }
                }
              else
                BLK_CUR_FILEPOS = filepos;
            }

          off = filepos & (SECTOR_SIZE - 1);
          size = ((BLK_BLKLENGTH (BLK_CUR_BLKLIST) - BLK_CUR_BLKNUM)
                  * SECTOR_SIZE) - off;
          if (size > len)
            size = len;

          disk_read_func = disk_read_hook;

          /* read current block and put it in the right place in memory */
          devread (BLK_BLKSTART (BLK_CUR_BLKLIST) + BLK_CUR_BLKNUM,
                   off, size, buf);

          disk_read_func = NULL;

          len -= size;
          filepos += size;
          ret += size;
          buf += size;
        }

      if (errnum)
        ret = 0;

      return ret;
    }
#endif /* NO_BLOCK_FILES */

  if (fsys_type == NUM_FSYS)
    {
      errnum = ERR_FSYS_MOUNT;
      return 0;
    }

  return (*(fsys_table[fsys_type].read_func)) (buf, len);
}

#ifndef STAGE1_5
/* Reposition a file offset.  */
int
grub_seek (int offset)
{
  if (offset > filemax || offset < 0)
    return -1;

  filepos = offset;
  return offset;
}

int
dir (char *dirname)
{
#ifndef NO_DECOMPRESSION
  compressed_file = 0;
#endif /* NO_DECOMPRESSION */

  if (!(dirname = setup_part (dirname)))
    return 0;

  if (*dirname != '/')
    errnum = ERR_BAD_FILENAME;

  if (fsys_type == NUM_FSYS)
    errnum = ERR_FSYS_MOUNT;

  if (errnum)
    return 0;

  /* set "dir" function to list completions */
  print_possibilities = 1;

  return (*(fsys_table[fsys_type].dir_func)) (dirname);
}
#endif /* STAGE1_5 */

void
grub_close (void)
{
#ifndef NO_BLOCK_FILES
  if (block_file)
    return;
#endif /* NO_BLOCK_FILES */

  if (fsys_table[fsys_type].close_func != 0)
    (*(fsys_table[fsys_type].close_func)) ();
}