#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netinet/in.h>
#include<arpa/inet.h>

int total_entry;

struct Routing_Table_Entry {
	unsigned long network_id;
	unsigned long int subnet_mask;
	unsigned long gateway;
	char interface[100];
	 
};


struct Routing_Table_Entry Routing_Table [100];


void my_sort(int);
void insertEntries(unsigned long,unsigned long int,unsigned long,char*,int);
void display();
void compare(FILE *);


void insertEntries(unsigned long network_id, unsigned long int subnet_mask, unsigned long gateway, char interface[100], int index)
{
        
        
        Routing_Table[index].network_id = network_id;
        Routing_Table[index].subnet_mask = subnet_mask;
        Routing_Table[index].gateway = gateway;
        strcpy(Routing_Table[index].interface,interface);
      
        
}

void compare(FILE *file1)
{
      
	
        unsigned long converted_ip;
      	int i=0;
        char ip[20],buffer[500];
	printf("\n Matched entries\n");
	printf("\n Network_id\t Subnet Mask\t Gateway\tInterface\n");
        while(fgets(buffer,500,file1)!= NULL)
        {
        	sscanf(buffer,"%s",ip);
            	converted_ip=inet_addr(ip);
            
		for(i=0;i<total_entry;i++)
            	{
           	 	if((converted_ip&Routing_Table[i].subnet_mask)==Routing_Table[i].network_id)
            		{
            			printf("\n %s",inet_ntoa(*(struct in_addr *)&Routing_Table[i].network_id));
                	 	printf("\t %s",inet_ntoa(*(struct in_addr *)&Routing_Table[i].subnet_mask));
                	 	printf("\t %s",inet_ntoa(*(struct in_addr *)&Routing_Table[i].gateway));
                	 	printf("\t %s\n",Routing_Table[i].interface);
         			break;
            		}
            	}
        }
     
}



void parser(FILE* fp)
{

	//FILE *fp;
	//fp = fopen(name, "r");
	char entries[500];
        int counter=0;
        char interface[100];
        unsigned long network_id;
        unsigned long int subnet_mask ;
        unsigned long gateway;
        //unsigned long gateway;
		 while(fgets(entries,500,fp)!= NULL)
		 {
			network_id = inet_addr(strtok(entries, ", "));
			subnet_mask = inet_addr(strtok(NULL, ", "));
		 	gateway = inet_addr(strtok(NULL, ", "));
			memcpy(interface, strtok(NULL, ", "), 10);
			
                        insertEntries(network_id, subnet_mask, gateway, interface, counter);
                        
                        counter++;
		}
		
		total_entry=counter;
		
		my_sort(counter);
		
		fclose(fp);
	
}

void my_sort(int index)
{
	int i=0,j=0;
	struct Routing_Table_Entry temp[500];
	for(i=0;i<index;i++)
	{
		for(j=i+1;j<=index-1;j++)
		{
			if(Routing_Table[i].subnet_mask <= Routing_Table[j].subnet_mask)
			{
				temp[i]=Routing_Table[i];
				Routing_Table[i]=Routing_Table[j];
				Routing_Table[j]=temp[i];
				
			}
		}
		
	}
}

void display()
{
	int index=0;
	printf("\n %d",total_entry);
	printf("\n Network Id\t\t Subnet_mast \t\t Gateway\t\t Interface");
	for(index = 0; index<total_entry; index++)
	{
		printf("\n count %d",index);
		printf("\nnetwork id: %s",inet_ntoa(*(struct in_addr *)&Routing_Table[index].network_id));
                printf("\tmask: %s",inet_ntoa(*(struct in_addr *)&Routing_Table[index].subnet_mask));
               	printf("\tgate way: %s",inet_ntoa(*(struct in_addr *)&Routing_Table[index].gateway));
               	printf("\tInterface: %s\n",Routing_Table[index].interface);
	        
	    
	        
	}
	
}

int main(int argc,char *argv[])
{
   
    FILE *fp,*ip;

   // printf("File name = %s\n", argv[1]);
    fp=fopen(argv[1],"r");
    
    if(fp==NULL) 
    { 
        perror("File cannot be opened");
      
       
        exit(1);
    }
    else 
    {
      	parser(fp);
      
        fclose(fp);
    }
    ip=fopen(argv[2],"r");
    
    if(ip==NULL) 
    { 
        perror("File cannot be opened");
      
        exit(1);
    }
    else 
    {
      	compare(ip);
      
        fclose(ip);
    }
    
   return(0);    
} 
