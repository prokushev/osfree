#ifndef SOM_Module_lnk_Source
#define SOM_Module_lnk_Source
#endif
#define LnkEmitter_Class_Source

#include "lnkemit.xih"
#include <scmodule.xh>
#include <scclass.xh>
#include <sctmplt.xh>
#include <string.h>
#include <stdio.h>


boolean flag=FALSE; // �ᯮ������, �⮡ ᥪ�� Prolog ��ࠡ��뢠���� ���� ࠧ.

/*
 * ��ࠢ����� ��᫥����⥫쭮���� ᥪ権
 */

SOM_Scope boolean  SOMLINK somtGenerateSections(LnkEmitter *somSelf)
{
  char *DllName;
  char *obj;
  char buf[1024];
  SOMTTemplateOutputC   *templ = somSelf->_get_somtTemplate();
  SOMTClassEntryC       *cls = somSelf->_get_somtTargetClass();
//  LnkEmitterData     *EmitterData = LnkEmitterGetData(somSelf);

  LnkEmitterMethodDebug("LnkEmitter","somtGenerateSections");

 if ( cls )
 {
   DllName=somSelf->somtGetGlobalModifierValue("dllname");
   if (!DllName) DllName= cls->somtGetModifierValue("dllname");
   if (!DllName) DllName= cls->somtGetModifierValue("filestem");
   strcpy(buf, DllName);
   templ->somtSetSymbolCopyBoth("lnkDLLName", buf );
 };

 if ( cls )
 {
   obj = somSelf->somtGetGlobalModifierValue("file");
   templ->somtSetSymbolCopyBoth("lnkObjects", obj);

   obj = somSelf->somtGetGlobalModifierValue("include");

   if (obj)
   {
    FILE *fp;
    char *content;

     fp = fopen(obj, "r" );
     if( fp != NULL )
     {
       long length = 0;
       fseek(fp, 0, SEEK_END);
       length = ftell(fp);
       fseek(fp, 0, SEEK_SET);

       content=(char *)malloc(length);
       fread(content, length, 1, fp);
       fclose(fp);
       templ->somtSetSymbolCopyBoth("lnkInclude", content);
       free(content);
     }
   }

   obj = somSelf->somtGetGlobalModifierValue("lib");
   templ->somtSetSymbolCopyBoth("lnkLibs", obj);
 };

  somSelf->somtFileSymbols();

  if (!flag)
  {
    somSelf->somtEmitProlog();
    flag=TRUE;
  }


  if ( cls )
  {
    somSelf->somtEmitClass();
  }

  return TRUE;
}
