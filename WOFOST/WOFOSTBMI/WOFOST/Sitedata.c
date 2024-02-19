#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "site.h"


void GetSiteData(Field *SITE, char *sitefile)
{
  TABLE *Table[NR_TABLES_SITE], *start;
  
  char line[MAX_STRING];
  int i, c, count;
  float Variable[NR_VARIABLES_SITE], XValue, YValue;
  char x[2], xx[2],  word[100];
  FILE *fq;

 if ((fq = fopen(sitefile, "rt")) == NULL) {
     fprintf(stderr, "Cannot open input file %s.\n", sitefile); 
     exit(0);
 }

 i=0;
 while ((c=fscanf(fq,"%s",word)) != EOF) 
 {
    if (strlen(word)> 98) 
    {
        fprintf(stderr, "Check the crop input file: very long strings.\n"); 
        exit(0);
    }
    if (!strcmp(word, SiteParam[i])) {
        while ((c=fgetc(fq)) !='=');
        
    fscanf(fq,"%f",  &Variable[i]);

    i++; 
      }  
 }

  if (i != NR_VARIABLES_SITE) {
      fprintf(stderr, "Something wrong with the Site variables in file %s.\n", sitefile);
      exit(0);
  }
  rewind(fq);  

  FillSiteVariables(SITE, Variable);
 
  i=0;
  count = 0;
    while (strcmp(SiteParam2[i],"NULL")) {
        while(fgets(line, MAX_STRING, fq)) {
            if(line[0] == '*' || line[0] == ' ' || line[0] == '\n' || line[0] == '\r'){
                continue;
            }
            
            sscanf(line,"%s",word);
            if (!strcmp(word, SiteParam2[i])) {
                
                c = sscanf(line,"%s %s %f %s  %f", word, x, &XValue, xx, &YValue);
                
                Table[i]= start= malloc(sizeof(TABLE));
                Table[i]->next = NULL;      
                Table[i]->x = XValue;
                Table[i]->y = YValue;            

                while (fgets(line, MAX_STRING, fq)) {  
                    if((c = sscanf(line," %f %s  %f",  &XValue, xx, &YValue)) != 3) break;
                    
                    Table[i]->next = malloc(sizeof(TABLE));
                    Table[i] = Table[i]->next; 
                    Table[i]->next = NULL;
                    Table[i]->x = XValue;
                    Table[i]->y = YValue;
                }
                /* Go back to beginning of the table */
                Table[i] = start;
                count++;
                break;
            }
        } 
        rewind(fq);    
    i++; 
    }
  
  fclose(fq);

  if (count != NR_TABLES_SITE)
  {
    fprintf(stderr, "Something wrong with the Site tables in file %s.\n", sitefile);
    exit(0);
  } 
 
  SITE->NotInfTB = Table[0];
 
}

