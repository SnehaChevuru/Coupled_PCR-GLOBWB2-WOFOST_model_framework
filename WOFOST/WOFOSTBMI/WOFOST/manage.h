#ifndef MANAGE_H
#define MANAGE_H

char *ManageParam[]={"NRFTAB",
                   "PRFTAB",
                   "KRFTAB",
                   "NMINS",
                   "RTNMINS",
           "PMINS",
           "RTPMINS",
           "KMINS",
           "RTKMINS",
                   "NULL"
           };

char *ManageParam2[]={
                    "FERNTAB",
                    "FERPTAB",
                    "FERKTAB",
                    "IRRTAB",
                    "NULL"
            };

extern void FillManageVariables();
extern int FillAfgenTables();

#endif //MANAGE_H
