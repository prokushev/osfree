
/*
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *     SOM incremental update: 2.41
 */


/*
 * somtest.c:
 */

/*
 *      Copyright (C) 2003 Ulrich M�ller.
 *
 *      This file is part of the XWorkplace source package.
 *      XWorkplace is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published
 *      by the Free Software Foundation, in version 2 as it comes in the
 *      "COPYING" file of the XWorkplace main distribution.
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 */

#include <stdio.h>
#include <os2.h>

#include "setup.h"

#include "helpers\stringh.h"

// this #define causes the *.ih file to actually define
// functions and variables; without it, the *.ih only
// declares things
#define XWPTest_Class_Source

#include <somtest.ih>

/* ******************************************************************
 *
 *   Methods
 *
 ********************************************************************/

SOM_Scope long  SOMLINK t_test1(XWPTest *somSelf, long lBlah)
{
    XWPTestData *somThis = XWPTestGetData(somSelf);
    XWPTestMethodDebug("XWPTest","t_test1");

    printf("  test method 1 called with arg %d\n", lBlah);

    return 1;
}

SOM_Scope long  SOMLINK t_test2(XWPTest *somSelf, long lBlah)
{
    XWPTestData *somThis = XWPTestGetData(somSelf);
    XWPTestMethodDebug("XWPTest","t_test2");

    printf("  test method 2 called with arg %d\n", lBlah);

    return 2;
}

SOM_Scope void SOMLINK t_somDefaultInit(XWPTest *somSelf, somInitCtrl* ctrl)
{
    XWPTestData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    XWPTestMethodDebug("XWPTest","somDefaultInit");
    XWPTest_BeginInitializer_somDefaultInit;

    XWPTest_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /*
     * local XWPTest initialization code added by programmer
     */

    _longvar = 0xa0a0a0a0;
    _pvoid = (PVOID)0xb0b0b0b0;

    printf("  init: somSelf is        0x%08lX\n", somSelf);
    printf("  init: somThis is        0x%08lX\n", somThis);
}


SOM_Scope void SOMLINK t_somDestruct(XWPTest *somSelf, octet doFree,
                                     somDestructCtrl* ctrl)
{
    XWPTestData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    XWPTestMethodDebug("XWPTest","t_somDestruct");
    XWPTest_BeginDestructor;

    /*
     * local XWPTest deinitialization code added by programmer
     */

    XWPTest_EndDestructor;
}

/* ******************************************************************
 *
 *   Helpers
 *
 ********************************************************************/

   APIRET APIENTRY  DosQueryModFromEIP(HMODULE *phMod,
                                        ULONG *pObjNum,
                                        ULONG BuffLen,
                                        PCHAR pBuff,
                                        ULONG *pOffset,
                                        ULONG Address);

/* ******************************************************************
 *
 *   main()
 *
 ********************************************************************/

CHAR    G_szName[CCHMAXPATH];

PCSZ CheckPtr(PVOID p)
{
    HMODULE hmod;
    ULONG   obj;
    ULONG   ofs = 0;
    APIRET  arc;

    if (!p)
        return "null";
    if (p == _XWPTest)
        return "_XWPTest";
    if (p == &XWPTestClassData)
        return "&XWPTestClassData";
    if (p == &XWPTestCClassData)
        return "&XWPTestCClassData";
    if (p == &XWPTestCClassData.parentMtab)
        return "&XWPTestCClassData.parentMtab";
    if (p == (PVOID)&t_test1)
        return "&t_test1";
    if (p == (PVOID)&t_test2)
        return "&t_test2";
    if (p == (PVOID)&t_somDefaultInit)
        return "&t_somDefaultInit";
    if (p == (PVOID)&t_somDestruct)
        return "&t_somDestruct";
    if (p == _SOMObject)
        return "_SOMObject";
    if (p == _SOMClass)
        return "_SOMClass";

    G_szName[0] = '\0';
    if (!(arc = DosQueryModFromEIP(&hmod,
                                   &obj,
                                   sizeof(G_szName), G_szName,
                                   &ofs,
                                   (ULONG)p)))
    {
        sprintf(G_szName + strlen(G_szName),
                " %lX:%08lX",
                obj,
                ofs);
        return G_szName;
    }

    return "unknown";
}

VOID DumpObj(SOMObject *pobj)
{

#if 0
    typedef struct somMethodTabStruct {
        SOMClass        *classObject;
        somClassInfo    *classInfo;
        char            *className;
        long        instanceSize;
        long        dataAlignment;
        long        mtabSize;
        long        protectedDataOffset; // from class's introduced data
        somDToken       protectedDataToken;
        somEmbeddedObjStruct *embeddedObjs;
        // remaining structure is opaque
        somMethodProc* entries[1];
    } somMethodTab, *somMethodTabPtr;

        typedef struct somStaticClassInfoStruct {
            unsigned long layoutVersion;  /* this struct defines layout version SOM_SCILEVEL */
            unsigned long numStaticMethods;   /* count of smt entries */
            unsigned long numStaticOverrides; /* count of omt entries */
            unsigned long numNonInternalData; /* count of nit entries */
            unsigned long numProcMethods;     /* count of pmt entries */
            unsigned long numVarargsFuncs;    /* count of vft entries */
            unsigned long majorVersion;
            unsigned long minorVersion;
            unsigned long instanceDataSize;   /* instance data introduced by this class */
            unsigned long maxMethods;         /* count numStaticMethods and numMethods */
            unsigned long numParents;
            somId    classId;
            somId    explicitMetaId;
            long implicitParentMeta;
            somId                  *parents;
            somClassDataStructure  *cds;
            somCClassDataStructure *ccds;
            somStaticMethod_t      *smt; /* basic "static" methods for mtab */
            somOverrideMethod_t    *omt; /* overrides for mtab */
            char                   *nitReferenceBase;
            somNonInternalData_t   *nit; /* datatokens for instance data */
            somProcMethods_t       *pmt; /* Arbitrary ClassData members */
            somVarargsFuncs_t      *vft; /* varargs stubs */
            somTP_somClassInitFunc *cif; /* class init function */
            /* end of layout version 1 */

            /* begin layout version 2 extensions */
            long dataAlignment; /* the desired byte alignment for instance data */
            /* end of layout version 2 */

        #define SOMSCIVERSION 1

            /* begin layout version 3 extensions */
            long numDirectInitClasses;
            somId *directInitClasses;
            unsigned long numMethods; /* general (including nonstatic) methods for mtab */
            somMethods_t       *mt;
            unsigned long protectedDataOffset; /* access = resolve(instanceDataToken) + offset */
            unsigned long somSCIVersion;  /* used during development. currently = 1 */
            unsigned long numInheritedMethods;
            somInheritedMethod_t *imt; /* inherited method implementations */
            unsigned long numClassDataEntries; /* should always be filled in */
            somId *classDataEntryNames; /* either NULL or ptr to an array of somIds */
            unsigned long numMigratedMethods;
            somMigratedMethod_t *mmt; /* migrated method implementations */
            unsigned long numInitializers; /* the initializers for this class */
            somId *initializers;     /* in order of release */
            unsigned long somDTSClass; /* used to identify a DirectToSOM class */
            somDynamicSCI *dsci;  /* used to register dynamically computed info */
            /* end of layout version 3 */

        } somStaticClassInfo, *somStaticClassInfoPtr;
#endif

    // the first ULONG pointed to by an object pointer is
    // actually a pointer to a somMethodTab struct; after
    // that ULONG follows the object's instance data
    PULONG pul = (PULONG)pobj;
    ULONG ul, c;
    somMethodTab *pTab = (somMethodTab*)*pul;

    printf("dumping somMethodTab at 0x%08lX (pointed to by first ULONG at 0x%08lX)\n",
            pTab, pobj);

    printf("%04lX         classObject: 0x%08lX (%s)\n", (PBYTE)&pTab->classObject - (PBYTE)pTab,
            pTab->classObject, CheckPtr(pTab->classObject));
    printf("%04lX           classInfo: 0x%08lX (%s)\n", (PBYTE)&pTab->classInfo - (PBYTE)pTab,
            pTab->classInfo, CheckPtr(pTab->classInfo));

    printf("%04lX           className: 0x%08lX (\"%s\")\n",  (PBYTE)&pTab->className - (PBYTE)pTab,
            pTab->className, pTab->className);
    printf("%04lX        instanceSize: 0x%08lX (%d)\n", (PBYTE)&pTab->instanceSize - (PBYTE)pTab,
            pTab->instanceSize, pTab->instanceSize);
    printf("%04lX       dataAlignment: 0x%08lX (%d)\n", (PBYTE)&pTab->dataAlignment - (PBYTE)pTab,
            pTab->dataAlignment, pTab->dataAlignment);
    printf("%04lX            mtabSize: 0x%08lX (%d)\n", (PBYTE)&pTab->mtabSize - (PBYTE)pTab,
            pTab->mtabSize, pTab->mtabSize);
    printf("%04lX protectedDataOffset: 0x%08lX (%d)\n", (PBYTE)&pTab->protectedDataOffset - (PBYTE)pTab,
            pTab->protectedDataOffset, pTab->protectedDataOffset);
    printf("%04lX  protectedDataToken: 0x%08lX (%d)\n", (PBYTE)&pTab->protectedDataToken - (PBYTE)pTab,
            pTab->protectedDataToken, pTab->protectedDataToken);
    printf("%04lX        embeddedObjs: 0x%08lX (%s)\n", (PBYTE)&pTab->embeddedObjs - (PBYTE)pTab,
            pTab->embeddedObjs, CheckPtr(pTab->embeddedObjs));
    for (ul = (PBYTE)&pTab->entries - (PBYTE)pTab, c = 0;
         ul < pTab->mtabSize;
         ul += 4, c++)
    {
        printf("%04lX          method %03d: 0x%08lX (%s)\n", ul,
                c, pTab->entries[c], CheckPtr(pTab->entries[c]));
    }
}

/*
 * main:
 */

int main(int argc, char *argv[])
{
    SOMObject   *pobj1,
                *pobj2;
    SOMClass    *pClass;
    PULONG      pul;

    somTD_XWPTest_test1 pMethod1,
                        pMethod2,
                        pMethod3;

    pMethod1 = t_test1;

    printf("\n\n");

    printf("creating XWPTest object...\n");

    pobj1 = XWPTestNew();
    printf("created XWPTest object 1 at 0x%08lX\n", pobj1);
    pobj2 = XWPTestNew();
    printf("created XWPTest object 2 at 0x%08lX\n", pobj2);

    printf("\n");

    printf("              _SOMObject: 0x%08lX\n", _SOMObject);
    printf("               _SOMClass: 0x%08lX\n", _SOMClass);
    printf("                _XWPTest: 0x%08lX\n", _XWPTest);
    printf(" ..CClassData.parentMtab: 0x%08lX\n", XWPTestCClassData.parentMtab);
    printf("     XWPTest static mtab: 0x%08lX\n", &XWPTestSM);
    printf("       &XWPTestClassData: 0x%08lX\n", &XWPTestClassData);
    printf("      &XWPTestCClassData: 0x%08lX\n", &XWPTestCClassData);


    printf("\n");

    printf("testing object 1 (0x%08lX)\n", pobj1);
    DumpObj(pobj1);

    pClass = _somGetClass(pobj1);
    printf("      somGetClass(pobj1): 0x%08lX (%s)\n", pClass, CheckPtr(pClass));

    pul = (PULONG)pClass;
    printf("             *0x%08lX: 0x%08lX (%s)\n", pul, *pul, CheckPtr((PVOID)*pul));
    pClass = _somGetClass(pClass);
    printf("     somGetClass(pClass): 0x%08lX (%s)\n", pClass, CheckPtr(pClass));
    pul = (PULONG)pClass;
    printf("             *0x%08lX: 0x%08lX (%s)\n", pul, *pul, CheckPtr((PVOID)*pul));

    printf("testing object 2 (0x%08lX)\n", pobj2);
    pul = (PULONG)pobj2;
    printf("             *0x%08lX: 0x%08lX (%s)\n", pul, *pul, CheckPtr((PVOID)*pul));
    pul = (PULONG)*pul;
    printf("             *0x%08lX: 0x%08lX (%s)\n", pul, *pul, CheckPtr((PVOID)*pul));

    printf("\ntokens:\n");
    pul = XWPTestClassData.test1;
    printf("  XWPTestClassData.test1: 0x%08lX (%s)\n", pul, CheckPtr(pul));
    pul = XWPTestClassData.test2;
    printf("  XWPTestClassData.test2: 0x%08lX (%s)\n", pul, CheckPtr(pul));

    pMethod2 = SOM_Resolve(pobj1, XWPTest, test1);
    printf("       SOMResolve(test1): 0x%08lX (%s)\n", pMethod2, CheckPtr((PVOID)pMethod2));

    printf("             *0x%08lX: 0x%08lX (%s)\n", pul, *pul, CheckPtr((PVOID)pMethod2));

    printf("\nfunc pointers:\n");
    pMethod3 = (somTD_XWPTest_test1)somResolve(pobj1, XWPTestClassData.test1);
    printf("       somResolve(test1): 0x%08lX (%s)\n", pMethod3, CheckPtr((PVOID)pMethod3));
    printf("                &t_test1: 0x%08lX\n", &t_test1);

    _test1(pobj1, 3);
    pMethod2(pobj1, 4);
    t_test1(pobj1, 5);

    return 2;
}

