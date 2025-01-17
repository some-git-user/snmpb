/*
 * smi.h --
 *
 *      Interface Definition of libsmi (version 2:27:0).
 *
 * Copyright (c) 1999,2000 Frank Strauss, Technical University of Braunschweig.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * @(#) $Id: smi.h.in 8090 2008-04-18 12:56:29Z strauss $
 */

#ifndef _SMI_H
#define _SMI_H

#include <stdlib.h>
#include <stdarg.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_LIMITS_H
#include "limits.h"
#endif
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


#define SMI_LIBRARY_VERSION "2:27:0"
extern const char *smi_library_version;

#define SMI_VERSION_MAJOR 0
#define SMI_VERSION_MINOR 4
#define SMI_VERSION_PATCHLEVEL 8
#define SMI_VERSION_STRING "0.4.8"
extern const char *smi_version_string;



#define SMI_FLAG_NODESCR   0x0800 /* do not load descriptions/references.    */
#define SMI_FLAG_VIEWALL   0x1000 /* all modules are `known', need no views. */
#define SMI_FLAG_ERRORS    0x2000 /* print parser errors.                    */
#define SMI_FLAG_RECURSIVE 0x4000 /* recursively parse imported modules.     */
#define SMI_FLAG_STATS     0x8000 /* print statistics after parsing module.  */
#define SMI_FLAG_MASK      (SMI_FLAG_NODESCR|SMI_FLAG_VIEWALL|SMI_FLAG_STATS|\
			    SMI_FLAG_RECURSIVE|SMI_FLAG_ERRORS)



/* misc mappings of SMI types to C types                                     */
typedef char                    *SmiIdentifier;
typedef unsigned long           SmiUnsigned32;
typedef long                    SmiInteger32;
#ifdef _MSC_VER /* if using MSVC and not configure */
typedef __int64                 SmiInteger64;
typedef unsigned __int64        SmiUnsigned64;
#else
typedef unsigned long long           SmiUnsigned64;
typedef long long            SmiInteger64;
#endif
typedef unsigned int            SmiSubid;
typedef float                   SmiFloat32;
typedef double                  SmiFloat64;
typedef long double             SmiFloat128;



/* SmiLanguage -- language of an actual MIB module                           */
typedef enum SmiLanguage {
    SMI_LANGUAGE_UNKNOWN                = 0,  /* should not occur            */
    SMI_LANGUAGE_SMIV1                  = 1,
    SMI_LANGUAGE_SMIV2                  = 2,
    SMI_LANGUAGE_SMING                  = 3,
    SMI_LANGUAGE_SPPI                   = 4
} SmiLanguage;

/* SmiBasetype -- base types of all languages                                */
typedef enum SmiBasetype {
    SMI_BASETYPE_UNKNOWN                = 0,  /* should not occur            */
    SMI_BASETYPE_INTEGER32              = 1,  /* also SMIv1/v2 INTEGER       */
    SMI_BASETYPE_OCTETSTRING            = 2,
    SMI_BASETYPE_OBJECTIDENTIFIER       = 3,
    SMI_BASETYPE_UNSIGNED32             = 4,
    SMI_BASETYPE_INTEGER64              = 5,  /* SMIng and SPPI              */
    SMI_BASETYPE_UNSIGNED64             = 6,  /* SMIv2, SMIng and SPPI       */
    SMI_BASETYPE_FLOAT32                = 7,  /* only SMIng                  */
    SMI_BASETYPE_FLOAT64                = 8,  /* only SMIng                  */
    SMI_BASETYPE_FLOAT128               = 9,  /* only SMIng                  */
    SMI_BASETYPE_ENUM                   = 10,
    SMI_BASETYPE_BITS                   = 11, /* SMIv2, SMIng and SPPI       */
    SMI_BASETYPE_POINTER		= 12  /* only SMIng                  */
} SmiBasetype;

#ifdef INT32_MIN
#define SMI_BASETYPE_INTEGER32_MIN  INT32_MIN
#else
#define SMI_BASETYPE_INTEGER32_MIN  INT_MIN
#endif
#ifdef INT32_MAX
#define SMI_BASETYPE_INTEGER32_MAX  INT32_MAX
#else
#define SMI_BASETYPE_INTEGER32_MAX  INT_MAX
#endif
#define SMI_BASETYPE_INTEGER64_MIN  LIBSMI_INT64_MIN
#define SMI_BASETYPE_INTEGER64_MAX  LIBSMI_INT64_MAX
#define SMI_BASETYPE_UNSIGNED32_MIN 0
#ifdef UINT32_MAX
#define SMI_BASETYPE_UNSIGNED32_MAX UINT32_MAX
#else
#define SMI_BASETYPE_UNSIGNED32_MAX UINT_MAX
#endif
#define SMI_BASETYPE_UNSIGNED64_MIN 0
#define SMI_BASETYPE_UNSIGNED64_MAX LIBSMI_UINT64_MAX

/* SmiStatus -- values of status levels                                      */
typedef enum SmiStatus {
    SMI_STATUS_UNKNOWN          = 0, /* should not occur                     */
    SMI_STATUS_CURRENT          = 1, /* only SMIv2, SMIng and SPPI           */
    SMI_STATUS_DEPRECATED       = 2, /* SMIv1, SMIv2, SMIng and SPPI         */
    SMI_STATUS_MANDATORY        = 3, /* only SMIv1                           */
    SMI_STATUS_OPTIONAL         = 4, /* only SMIv1                           */
    SMI_STATUS_OBSOLETE         = 5  /* SMIv1, SMIv2, SMIng and SPPI         */
} SmiStatus;

/* SmiAccess -- values of access levels                                      */
typedef enum SmiAccess {
    SMI_ACCESS_UNKNOWN          = 0, /* should not occur                     */
    SMI_ACCESS_NOT_IMPLEMENTED  = 1, /* only for agent capability variations */
    SMI_ACCESS_NOT_ACCESSIBLE   = 2, /* the values 2 to 5 are allowed to be  */
    SMI_ACCESS_NOTIFY           = 3, /* compared by relational operators.    */
    SMI_ACCESS_READ_ONLY        = 4,
    SMI_ACCESS_READ_WRITE       = 5,
    SMI_ACCESS_INSTALL          = 6, /* these three entries are only valid   */
    SMI_ACCESS_INSTALL_NOTIFY   = 7, /* for SPPI                             */
    SMI_ACCESS_REPORT_ONLY      = 8,
    SMI_ACCESS_EVENT_ONLY      	= 9	 /* this entry is valid only for SMIng	 */
} SmiAccess;

/* SmiNodekind -- type or statement that leads to a definition               */
typedef unsigned int SmiNodekind;
#define SMI_NODEKIND_UNKNOWN      0x0000     /* should not occur             */
#define SMI_NODEKIND_NODE         0x0001
#define SMI_NODEKIND_SCALAR       0x0002
#define SMI_NODEKIND_TABLE        0x0004
#define SMI_NODEKIND_ROW          0x0008
#define SMI_NODEKIND_COLUMN       0x0010
#define SMI_NODEKIND_NOTIFICATION 0x0020
#define SMI_NODEKIND_GROUP        0x0040
#define SMI_NODEKIND_COMPLIANCE   0x0080
#define SMI_NODEKIND_CAPABILITIES 0x0100
#define SMI_NODEKIND_ANY          0xffff

/* SmiDecl -- type or statement that leads to a definition                   */
typedef enum SmiDecl {
    SMI_DECL_UNKNOWN            = 0,  /* should not occur                    */
    /* SMIv1/v2 ASN.1 statements and macros */
    SMI_DECL_IMPLICIT_TYPE      = 1,
    SMI_DECL_TYPEASSIGNMENT     = 2,
    SMI_DECL_IMPL_SEQUENCEOF    = 4,	/* this will go away */
    SMI_DECL_VALUEASSIGNMENT    = 5,
    SMI_DECL_OBJECTTYPE         = 6,    /* values >= 6 are assumed to be */
    SMI_DECL_OBJECTIDENTITY     = 7,    /* registering an OID, see check.c */
    SMI_DECL_MODULEIDENTITY     = 8,
    SMI_DECL_NOTIFICATIONTYPE   = 9,
    SMI_DECL_TRAPTYPE           = 10,
    SMI_DECL_OBJECTGROUP        = 11, 
    SMI_DECL_NOTIFICATIONGROUP  = 12,
    SMI_DECL_MODULECOMPLIANCE   = 13,
    SMI_DECL_AGENTCAPABILITIES  = 14,
    SMI_DECL_TEXTUALCONVENTION  = 15,
    SMI_DECL_MACRO	        = 16,
    SMI_DECL_COMPL_GROUP        = 17,
    SMI_DECL_COMPL_OBJECT       = 18,
    SMI_DECL_IMPL_OBJECT        = 19,	/* object label in sth like "iso(1)" */
    /* SMIng statements */
    SMI_DECL_MODULE             = 33,
    SMI_DECL_EXTENSION          = 34,
    SMI_DECL_TYPEDEF            = 35,
    SMI_DECL_NODE               = 36,
    SMI_DECL_SCALAR             = 37,
    SMI_DECL_TABLE              = 38,
    SMI_DECL_ROW                = 39,
    SMI_DECL_COLUMN             = 40,
    SMI_DECL_NOTIFICATION       = 41,
    SMI_DECL_GROUP              = 42,
    SMI_DECL_COMPLIANCE         = 43,
    SMI_DECL_IDENTITY	        = 44,
    SMI_DECL_CLASS		= 45,
    SMI_DECL_ATTRIBUTE		= 46,
    SMI_DECL_EVENT		= 47
} SmiDecl;

/* SmiIndexkind -- actual kind of a table row's index method                 */
typedef enum SmiIndexkind {
    SMI_INDEX_UNKNOWN           = 0, 
    SMI_INDEX_INDEX             = 1,
    SMI_INDEX_AUGMENT           = 2,
    SMI_INDEX_REORDER           = 3,
    SMI_INDEX_SPARSE            = 4,
    SMI_INDEX_EXPAND            = 5
} SmiIndexkind;

/* SmiValue -- any single value; for use in default values and subtyping     */
typedef struct SmiValue {
    SmiBasetype             basetype;
    unsigned int	    len;         /* OID, OctetString, Bits           */
    union {
        SmiUnsigned64       unsigned64;
        SmiInteger64        integer64;
        SmiUnsigned32       unsigned32;
        SmiInteger32        integer32;
        SmiFloat32          float32;
        SmiFloat64          float64;
        SmiFloat128         float128;
        SmiSubid	    *oid;
        char                *ptr;	 /* OctetString, Bits                */
    } value;
} SmiValue;

/* SmiNamedNumber -- a named number; for enumeration and bitset types        */
typedef struct SmiNamedNumber {
    SmiIdentifier       name;
    SmiValue            value;
} SmiNamedNumber;

/* SmiRange -- a min-max value range; for subtyping of sizes or numbers      */
typedef struct SmiRange {
    SmiValue            minValue;
    SmiValue            maxValue;
} SmiRange;

/* SmiModule -- the main structure of a module                               */
typedef struct SmiModule {
    SmiIdentifier       name;
    char                *path;
    char                *organization;
    char                *contactinfo;
    char                *description;
    char                *reference;
    SmiLanguage		language;
    int                 conformance;
} SmiModule;

/* SmiRevision -- content of a single module's revision clause               */
typedef struct SmiRevision {
    time_t              date;
    char                *description;
} SmiRevision;

/* SmiImport -- an imported descriptor                                       */
typedef struct SmiImport {
    SmiIdentifier       module;
    SmiIdentifier       name;
} SmiImport;

/* SmiMacro -- the main structure of a SMIv1/v2 macro or SMIng extension     */
typedef struct SmiMacro {
    SmiIdentifier       name;
    SmiDecl             decl;
    SmiStatus           status;
    char                *description;
    char                *reference;
    char		*abnf; /* only for SMIng */
} SmiMacro;

/* SmiIdentity -- the main structure of a SMIng Identity.		     */
/* NOTE: Not to be confused with SMIv2 MODULE-IDENTITY */
typedef struct SmiIdentity {
    SmiIdentifier       name;
    SmiDecl             decl;
    SmiStatus           status;
    char                *description;
    char                *reference;
} SmiIdentity;

/* SmiType -- the main structure of a type definition (also base types)      */
/* also SMIng attributes      */
typedef struct SmiType {
    SmiIdentifier       name;
    SmiBasetype         basetype;
    SmiDecl             decl;
    char                *format;
    SmiValue            value;
    char                *units;
    SmiStatus           status;
    char                *description;
    char                *reference;
} SmiType;

/* SmiNode -- the main structure of any clause that defines a node           */
typedef struct SmiNode {
    SmiIdentifier       name;
    unsigned int	oidlen;
    SmiSubid		*oid;         /* array of length oidlen */
    SmiDecl             decl;
    SmiAccess           access;
    SmiStatus           status;
    char                *format;
    SmiValue            value;
    char                *units;
    char                *description;
    char                *reference;
    SmiIndexkind        indexkind;    /* only valid for rows */
    int                 implied;      /* only valid for rows */
    int                 create;       /* only valid for rows */
    SmiNodekind         nodekind;
} SmiNode;

/* SmiElement -- an item in a list (row index column, notification object)   */
typedef struct SmiElement {
#ifndef __GNUC__
    char dummy;		/* many compilers are unhappy with empty structures. */
#endif
    /* no visible attributes */
} SmiElement;

/* SmiOption -- an optional group in a compliance statement                  */
typedef struct SmiOption {
    char                *description;
} SmiOption;

/* SmiRefinement -- a refined object in a compliance statement               */
typedef struct SmiRefinement {
    SmiAccess           access;
    char                *description;
} SmiRefinement;

/* SmiClass -- main structure for SMIng class statement               */
typedef struct SmiClass {
    SmiIdentifier       name;
    SmiDecl             decl;
    SmiStatus           status;
    char                *description;
    char                *reference;
} SmiClass;

/* SmiClass -- main structure for class attribute               */
typedef struct SmiAttribute {
    SmiIdentifier       name;
    SmiBasetype         basetype;
    SmiDecl             decl;
    char                *format;
    SmiValue            value;
    char                *units;
    SmiStatus           status;
    char                *description;
    char                *reference;
    SmiAccess		access;
} SmiAttribute;


/* SmiEvent -- the main structure of a SMIng Event(part of class definition).*/
typedef struct SmiEvent {
    SmiIdentifier       name;
    SmiDecl             decl;
    SmiStatus           status;
    char                *description;
    char                *reference;
} SmiEvent;



extern int smiInit(const char *tag);

extern void smiExit(void);

extern void smiSetErrorLevel(int level);

extern int smiGetFlags(void);

extern void smiSetFlags(int userflags);

extern char *smiGetPath(void);

extern int smiSetPath(const char *path);

extern void smiSetSeverity(char *pattern, int severity);

extern int smiReadConfig(const char *filename, const char *tag);

extern char *smiLoadModule(const char *module);

extern int smiIsLoaded(const char *module);


typedef void (SmiErrorHandler) (char *path, int line, int severity, char *msg, char *tag);

extern void smiSetErrorHandler(SmiErrorHandler smiErrorHandler);


extern SmiModule *smiGetModule(const char *module);

extern SmiModule *smiGetFirstModule(void);

extern SmiModule *smiGetNextModule(SmiModule *smiModulePtr);
      
extern SmiNode *smiGetModuleIdentityNode(SmiModule *smiModulePtr);

extern SmiImport *smiGetFirstImport(SmiModule *smiModulePtr);

extern SmiImport *smiGetNextImport(SmiImport *smiImportPtr);

extern int smiIsImported(SmiModule *smiModulePtr,
			 SmiModule *importedModulePtr, char *importedName);

extern SmiRevision *smiGetFirstRevision(SmiModule *smiModulePtr);

extern SmiRevision *smiGetNextRevision(SmiRevision *smiRevisionPtr);

extern int smiGetRevisionLine(SmiRevision *smiRevisionPtr);



extern SmiIdentity *smiGetFirstIdentity(SmiModule *smiModulePtr);

extern SmiIdentity *smiGetNextIdentity(SmiIdentity *smiIdentityPtr);

extern SmiIdentity *smiGetParentIdentity(SmiIdentity *smiIdentityPtr);

extern int smiGetIdentityLine(SmiIdentity *smiIdentityPtr);

extern SmiModule *smiGetIdentityModule(SmiIdentity *smiIdentityPtr);

extern SmiIdentity *smiGetIdentity(SmiModule *smiModulePtr,char *identity);



extern SmiType *smiGetType(SmiModule *smiModulePtr, char *type);

extern SmiType *smiGetFirstType(SmiModule *smiModulePtr);

extern SmiType *smiGetNextType(SmiType *smiTypePtr);

extern SmiType *smiGetParentType(SmiType *smiTypePtr);

extern SmiModule *smiGetTypeModule(SmiType *smiTypePtr);

extern int smiGetTypeLine(SmiType *smiTypePtr);

extern SmiRange *smiGetFirstRange(SmiType *smiTypePtr);

extern SmiRange *smiGetNextRange(SmiRange *smiRangePtr);

extern int smiGetMinMaxRange(SmiType *smiType, SmiValue *min, SmiValue *max);

extern SmiNamedNumber *smiGetFirstNamedNumber(SmiType *smiTypePtr);

extern SmiNamedNumber *smiGetNextNamedNumber(SmiNamedNumber
					         *smiNamedNumberPtr);
					         
					         
extern SmiClass *smiGetFirstClass(SmiModule *smiModulePtr);

extern SmiClass *smiGetNextClass(SmiClass *smiClassPtr);

extern SmiClass *smiGetParentClass(SmiClass *smiClassPtr);

extern SmiModule *smiGetClassModule(SmiClass *smiClassPtr);

extern SmiClass *smiGetClass(SmiModule *smiModulePtr,char *attribute);

extern int smiGetClassLine(SmiClass *smiClassPtr);



extern SmiAttribute *smiGetAttribute(SmiClass *smiClassPtr, char *attribute);

extern SmiAttribute *smiGetFirstAttribute(SmiClass *smiClassPtr);

extern SmiAttribute *smiGetNextAttribute(SmiAttribute *smiAtrributePtr);

extern SmiType *smiGetAttributeParentType(SmiAttribute *smiAtrributePtr);

extern SmiClass *smiGetAttributeParentClass(SmiAttribute *smiAtrributePtr);

extern SmiAttribute *smiGetFirstUniqueAttribute(SmiClass *smiClassPtr);

extern SmiAttribute *smiGetNextUniqueAttribute(SmiAttribute *smiTypePtr);

extern int 		smiIsClassScalar(SmiClass *smiClassPtr);

extern SmiNamedNumber *smiGetAttributeFirstNamedNumber(SmiAttribute *smiAttributePtr);

extern SmiNamedNumber *smiGetAttributeNextNamedNumber(SmiNamedNumber
					         *smiNamedNumberPtr);
extern SmiRange *smiGetAttributeFirstRange(SmiAttribute *smiAttributePtr);

extern SmiRange *smiGetAttributeNextRange(SmiRange *smiRangePtr);

extern int smiGetAttributeLine(SmiAttribute *smiAttributePtr);


extern SmiEvent *smiGetEvent(SmiClass *smiClassPtr, char *attribute);

extern SmiEvent *smiGetFirstEvent(SmiClass *smiClassPtr);

extern SmiEvent *smiGetNextEvent(SmiEvent *smiEventPtr);

extern int smiGetEventLine(SmiEvent *smiEventPtr);


extern SmiMacro *smiGetMacro(SmiModule *smiModulePtr, char *macro);

extern SmiMacro *smiGetFirstMacro(SmiModule *smiModulePtr);

extern SmiMacro *smiGetNextMacro(SmiMacro *smiMacroPtr);

extern SmiModule *smiGetMacroModule(SmiMacro *smiMacroPtr);

extern int smiGetMacroLine(SmiMacro *smiMacroPtr);


extern SmiNode *smiGetNode(SmiModule *smiModulePtr, const char *name);

extern SmiNode *smiGetNodeByOID(unsigned int oidlen, SmiSubid oid[]);

extern SmiNode *smiGetFirstNode(SmiModule *smiModulePtr, SmiNodekind nodekind);

extern SmiNode *smiGetNextNode(SmiNode *smiNodePtr, SmiNodekind nodekind);

extern SmiNode *smiGetParentNode(SmiNode *smiNodePtr);

extern SmiNode *smiGetRelatedNode(SmiNode *smiNodePtr);

extern SmiNode *smiGetFirstChildNode(SmiNode *smiNodePtr);

extern SmiNode *smiGetNextChildNode(SmiNode *smiNodePtr);

extern SmiModule *smiGetNodeModule(SmiNode *smiNodePtr);

extern SmiType *smiGetNodeType(SmiNode *smiNodePtr);

extern int smiGetNodeLine(SmiNode *smiNodePtr);




extern SmiElement *smiGetFirstElement(SmiNode *smiNodePtr);

extern SmiElement *smiGetNextElement(SmiElement *smiElementPtr);

extern SmiNode *smiGetElementNode(SmiElement *smiElementPtr);



extern SmiOption *smiGetFirstOption(SmiNode *smiComplianceNodePtr);

extern SmiOption *smiGetNextOption(SmiOption *smiOptionPtr);

extern SmiNode *smiGetOptionNode(SmiOption *smiOptionPtr);

extern int smiGetOptionLine(SmiOption *smiOptionPtr);


extern SmiRefinement *smiGetFirstRefinement(SmiNode *smiComplianceNodePtr);

extern SmiRefinement *smiGetNextRefinement(SmiRefinement *smiRefinementPtr);

extern SmiNode *smiGetRefinementNode(SmiRefinement *smiRefinementPtr);

extern SmiType *smiGetRefinementType(SmiRefinement *smiRefinementPtr);

extern SmiType *smiGetRefinementWriteType(SmiRefinement *smiRefinementPtr);

extern int smiGetRefinementLine(SmiRefinement *smiRefinementPtr);


extern SmiElement *smiGetFirstUniquenessElement(SmiNode *smiNodePtr);

#define smiGetNextUniquenessElement(p) smiGetNextElement(p)

extern char *smiRenderOID(unsigned int oidlen, SmiSubid *oid, int flags);

extern char *smiRenderValue(SmiValue *smiValuePtr, SmiType *smiTypePtr,
			    int flags);

extern char *smiRenderNode(SmiNode *smiNodePtr, int flags);

extern char *smiRenderType(SmiType *smiTypePtr, int flags);

#define SMI_RENDER_NUMERIC   0x01 /* render as numeric values */
#define SMI_RENDER_NAME      0x02 /* render as names */
#define SMI_RENDER_QUALIFIED 0x04 /* render names with module prefix */
#define SMI_RENDER_FORMAT    0x08 /* render by applying the type's format if
				     type is given and format is present */
#define SMI_RENDER_PRINTABLE 0x10 /* render string values as a printable
				     string if all octets are isprint() */
#define SMI_RENDER_UNKNOWN   0x20 /* render even unknown items as strings
  				     ("<unknown>") so that we never get NULL */
#define SMI_RENDER_ALL       0xff /* render as `human friendly' as possible */

#define SMI_UNKNOWN_LABEL "<unknown>"

   
/*
 * The functions smiGetMaxSize() and smiGetMinSize() compute the
 * max size constraint on a given BITS, OCTET STRING or OBJECT
 * IDENTIFIER type. The functions recurse towards the top of the
 * type derivation tree.
 */

extern unsigned int smiGetMinSize(SmiType *smiType);
extern unsigned int smiGetMaxSize(SmiType *smiType);

/*
 * Two utility functions to pack and unpack instance identifiers.
 * The smiUnpack() function allocates the array of SmiValues and
 * the smiPack() function allocates the array of SmiSubids.
 */

extern int smiUnpack(SmiNode *row, SmiSubid *oid, unsigned int oidlen,
		     SmiValue **vals, int *valslen);

extern int smiPack(SmiNode *row, SmiValue *vals, int valslen,
		   SmiSubid **oid, unsigned int *oidlen);

/*
 * Two printf functions that allocate memory dynamically. The call has
 * to free the allocated memory.
 */

extern int smiAsprintf(char **strp, const char *format, ...);

extern int smiVasprintf(char **strp, const char *format, va_list ap);


/*
 * The functions smiMalloc() and friends are used within the library
 * for all memory allocations and deallocations. These functions are
 * simple wrappers around the standard malloc() and friends functions,
 * sometimes with some additional checking. We export these functions
 * because on some systems (e.g. Windows) it is necessary to allocate
 * / deallocate memory with the 'right' version of malloc() and
 * friends.
 */

#ifdef HAVE_DMALLOC_H

extern void *_smiMalloc(char *, int, size_t);
extern void *_smiRealloc(char *, int, void *ptr, size_t size);
extern char *_smiStrdup(char *, int, const char *s1);
extern char *_smiStrndup(char *, int, const char *s1, size_t n);
extern void _smiFree(char *, int, void *ptr);

#define	smiMalloc(s)	_smiMalloc(__FILE__, __LINE__, s)
#define	smiRealloc(p,s)	_smiRealloc(__FILE__, __LINE__, p, s)
#define	smiStrdup(s)	_smiStrdup(__FILE__, __LINE__, s)
#define	smiStrndup(s,n)	_smiStrndup(__FILE__, __LINE__, s, n)
#define	smiFree(p)	_smiFree(__FILE__, __LINE__, p)

#else
extern void *smiMalloc(size_t size);
extern void *smiRealloc(void *ptr, size_t size);
extern char *smiStrdup(const char *s1);
extern char *smiStrndup(const char *s1, size_t n);
extern void smiFree(void *ptr);
#endif

#ifdef __cplusplus
}
#endif


#endif /* _SMI_H */
