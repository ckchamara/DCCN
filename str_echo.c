#include	"utils.h"		/* Header file */
#include <stdio.h>
#include <stdlib.h>

//The considerable file -> str_echo.c
//[Refered file] This file is referd and mainly based on Practical 5 of DCCN3 module in (SLIIT)Srilanka institute of information technology.
/*
 *Group Members

 */

void str_echo(int sockfd)
{
	char selected_qty[2];
	char unit_prize[MAXLINE];
	char selected_prod_Id[3];
	char confirmation_b[2];
	char confirmation_p[2];

	int product_qty = 0;
	int comma_count = 0;
	int line_no = 0;
	int count;
	int	available_qty;
	int Available_int;
	int purchase_available_int;

	char product_Id[3];
	char selected_spec[10];
	char doc_id_c[3];
	char confirmation[3];
	char buy[1];

	ssize_t	n;
	char line[MAXLINE];
	char error[MAXLINE];

	//bool is_available = false;
	//bool is_atleast_one = false;

	char product_list[300];
	char buff_temp[255];
	char res[256];
	char res_out[1000];
	char buff[1000];
	char buffer[255];
	char text_line[255];
	char newline[255];

	
	char str[MAXLINE];
	char pid[MAXLINE];
	char product[MAXLINE];
	char Brand[MAXLINE];	
	char Available[MAXLINE]; 
	char Unit_price[MAXLINE];
	char formattedtext[MAXLINE];

	FILE * fPtr;
	FILE * fTemp;
	//FILE *fp;
	FILE *fp_spec;

		
			//PART 1
		//WELCOME MESSAGE
		
	FILE *fp1 = fopen("welcome_message.txt", "r"); //open txt file and save txt file in file descriptor fp1
	if(fp1==NULL){
		snprintf(res_out, sizeof(res_out),"cant open file");
		return 1;
	}
	while(fgets(line,sizeof(line),fp1) != NULL) //FGETS reads the txt file and put it into line[] arrray,
	{
		n = strlen(line);//GET THE SIZE OF THE ARRAY
		writen(sockfd,line,n);//writing to socket
	
	}
	fclose(fp1);  // close file discriptor


					//PART  2---- 
	//GET THE PRODUCT TYPE FIRST 4 LETTERS AND DISPLAY PRODUCT DETAILS
	
	
	for ( ; ; ) {    //NEVER ENDING LOOP

		if ((n = Readline(sockfd, line, MAXLINE)) == 0)	//READ SERVER SOCKET AND CATCH CLIENT GIVEN PRODUCTS 1ST 4 LETTERS AND PUT IT INTO LINE ARRAY
			return;		/* connection closed by other end */

		//select product type
		if (sscanf(line, "%s", &selected_spec) == 1) //GET THE FIRST 4 LETTRS FROM LINE ARRAY AND COPY IT TO ANOTHER ARRAY
		{
			//check product type length
			if (strlen(selected_spec) == 4)//check 4 letters to Blender
			{
				if( strcmp(selected_spec ,"blen") == 0 || strcmp(selected_spec,"BLEN") == 0)//strcmp is useed to compare 2 values is equal
				{
					snprintf(selected_spec, sizeof(selected_spec),"Blender");//copy "blender" word to selected_spec[] array
				}
				else if(strcmp(selected_spec,"came") == 0  || strcmp(selected_spec,"CAME") == 0)
				{
					snprintf(selected_spec, sizeof(selected_spec),"Camera");
				}
				else if(strcmp(selected_spec,"heat") == 0 || strcmp(selected_spec,"HEAT") == 0)
				{
					snprintf(selected_spec, sizeof(selected_spec),"Heater");
				}
				else
				{
					bzero(line,sizeof(line)); //reset line[] array to zero
					snprintf(line, sizeof(line),"Invalid characters ,Please try again!(example:- blen) \n");	
					n = strlen(line);
					Writen(sockfd,line,n); //writing to socket
				}

				fp_spec = fopen("Stock.txt", "r");

				snprintf(line,sizeof(line),"PID\tBrandName\tQty\tU.Price\n");
				n= strlen(line);			
				Writen(sockfd,line,n);

			 	//printf("pid 	Brand	Available 	Unit_price\n" );
				while(fgets(product_list,MAXLINE,fp_spec)!=NULL)
				{
					sscanf(product_list, "%s\t%s\t%s\t%s\t%s", pid, product,Brand,Available,Unit_price); //scan txt file line by line and save respective fields seperated by tab into varieables.

					if (strcmp(product,selected_spec)==0)//string comparison/ this is equal to if(product==selected_spec)
					{
						snprintf(line,sizeof(line),"%s  %s  	%s  	%s\n",pid,Brand,Available,Unit_price);//put these varieable values to line[] array
						n = strlen(line);
						Writen(sockfd,line,n);
					}

				}
				bzero(line,sizeof(line));
				fclose(fp_spec);
				
				
				
					//PART  3
			//BUYING PART (WHEN USER GIVES - <b> <PROD_ID> <QTY> )
					
				for ( ; ; ) 
				{
					if ((n = Readline(sockfd, line, MAXLINE)) == 0)
						return;		/* connection closed by other end */

							
					if (sscanf(line,"%s %s %s", &confirmation_b , &selected_prod_Id, &selected_qty) == 3)
					{
						int selected_qty_int = atoi(selected_qty);

						//accept code further validation (length)
						if(strcmp(confirmation_b,"b") == 0)
						{
							FILE *fp;
							fp = fopen("Stock.txt","r");

							if(fp == NULL)
							{
								bzero(line,sizeof(line));
								snprintf(line,sizeof(line),"item file open error\n");
								writen(sockfd,line,sizeof(line));
							}
							else
							{
								while(Fgets(line,sizeof(line),fp)!=NULL)
								{									
									sscanf(line, "%s\t%s\t%s\t%s\t%s", &pid, &product,&Brand,&Available,&unit_prize);
									int unit_prize_int = atoi(unit_prize);
									Available_int = atoi(Available);
									
									if(strcmp(selected_prod_Id, pid) == 0)//STRING COMPARISON
									{
										if (selected_qty_int<=Available_int)
										{
											purchase_available_int=Available_int; //send this value to purchasing part
											char buff[MAXLINE];
											int i = selected_qty_int*unit_prize_int; //multiply selected qty * unit prize
											snprintf(buff, sizeof(buff),"Total Cost : %d.00 Press “P” to confirm the purchase\n",i);
											writen(sockfd,buff,strlen(buff));
										}
										else
											snprintf(error, sizeof(error), "Available quantity exceeded!\n");	
									}
										
								}
								n = strlen(error);
								Writen(sockfd, error, n); //write line

								rewind(fp); //reload file descriptor
								fclose(fp);
								
							}
								

									//PART  3

						//PURCHASING PART (WHEN USER GIVES - <p> <PROD_ID> <QTY> )
							for ( ; ; ) 
							{
								if ((n = Readline(sockfd, line, MAXLINE)) == 0)
									return;

								if (sscanf(line,"%s %s %s", &confirmation_p , &selected_prod_Id, &selected_qty) == 3)
								{
									if(strcmp(confirmation_p, "p") == 0)
									{
										int purchase_qty_int = atoi(selected_qty); //convert string to int by FUNCTION atoi()

										if (purchase_qty_int<=purchase_available_int) //purchase_available_int from Buy part
										{
											int pid =atoi(selected_prod_Id);
											int updatedqty=purchase_available_int- purchase_qty_int;
											snprintf(line, sizeof(line), "Your order processed successfully!\n");
											//updatedb(pid,updatedqty);

										}
										else
											snprintf(line, sizeof(line), "cant purchase,Available quantity exceeded!\n");
									}

								}
								else
									snprintf(line, sizeof(line), "check input systax is right!\n");

								n = strlen(line);
								Writen(sockfd, line, n); //write line
							}
								
							
						}
						else	
							snprintf(line, sizeof(line), "Accept code does not meet the correct format 111111111111!\n");	
						

							
					}
					else
						snprintf(line, sizeof(line), "Accept code does not meet the correct format 55555555555555!\n");
					
					n = strlen(line);
					Writen(sockfd, line, n); //write line		

				}
				n = strlen(line);
					Writen(sockfd, line, n); //write line

			}
			else
				snprintf(line, sizeof(line), "Product code error!\n");
		}
		else
			snprintf(line, sizeof(line), "Product code error!\n");

		n = strlen(line);
		Writen(sockfd, line, n); //write line
	}
}



//THIS CODE NOT WORKING
void updatedb (int selected_prod_Id, int qtyafter) {
    FILE * fp_read;
	FILE * fp_write;

    int pid;
    char product[MAXLINE],Brand[MAXLINE],Available[MAXLINE],unit_prize[MAXLINE],line[MAXLINE];

    fp_read = fopen ("Stock.txt", "r");
    fp_write = fopen ("temp.txt", "w+");
   

    while(Fgets(line,sizeof(line),fp_read)!=NULL)
    {	
        							
        sscanf(line, "%d\t%s\t%s\t%s\t%s", pid, &product,&Brand,&Available,&unit_prize);
        if(strcmp(selected_prod_Id, pid) == 0)
        {
            // code to logic //
            fprintf(fp_write, "%s\t%s\t%s\t%d\t%s", pid, product,Brand,qtyafter,unit_prize);
        }

        fprintf(fp_write, "%s\t%s\t%s\t%s\t%s", pid, product,Brand,Available,unit_prize);
        
    }
   
   fclose(fp_read);
   fclose(fp_write);
   
}
