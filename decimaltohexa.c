#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Global variable that will hold the byte ordering type.
char byteOrdType[25]="\0";

//Function that converts floating point numbers according to sizeof floats
char *floating(long double floating_number,short sizeOfFloats,char bin[]){
	// Separating floating number to its integer and fraction part
	double intpart;
	double fractionpart;
	fractionpart = modf(floating_number,&intpart);
	int integerpart = (int)intpart;
	if(fractionpart<0){
		fractionpart = -fractionpart;
	}
	int i;
	int arraysize = 0;
	int tempint = abs(integerpart);
	// Calculating arraysize for integer part
	for(i=0;tempint>0;i++){
		tempint=tempint/2;
		arraysize++;
	}
	int binaryinteger[arraysize];
	
	int temppint = abs(integerpart);
	// Calculating binary form of the integer part
	for(i=0;temppint>0;i++){
		binaryinteger[i]=temppint%2;
		temppint=temppint/2;
	}
	// Calculating the binary form of the fraction part.
	double tempfraction = fractionpart*2;
	int binaryfraction[100];
	double tempintpart;
	double tempfracpart;
	i=0;
	while(tempfracpart>0){
		tempfracpart = modf(tempfraction,&tempintpart);
		tempfraction = tempfracpart;
		int a = (int)tempintpart;
		if(tempintpart == 0){
			binaryfraction[i] = 0;
		}
		else if(tempintpart == 1){
			binaryfraction[i] = 1;
		}
		tempfraction = tempfraction*2;
		if(tempfracpart != 0){
			i++;
		}	
	}
	int arraycontentnumber = i;
	int exponent;
	// Calculating exponent.
	if(arraysize != 0){
		exponent = arraysize-1;
	}
	else if(arraysize == 0){
		int j;
		for(j=0;j<=arraycontentnumber;j++){
			if(binaryfraction[j] == 1){
				exponent = (j+1)*-1;
				break;
			}
		}
	}
	int mantissasize = arraycontentnumber+1+exponent;
	int mantissa[mantissasize];
	// Calculating the mantisssa for the number that is not between -1 and 1
	if(arraysize != 0){
		int index = 0;
		int k;
		for(k=arraysize-2;k>=0;k--){
			mantissa[index] = binaryinteger[k];
			index++;
		}
		for(k=0;k<=arraycontentnumber;k++){
			mantissa[index] = binaryfraction[k];
			index++;
		}
	}
	// Calculating the mantissa for the number that is between -1 and 1
	if(arraysize == 0){
		int index = 0;
		int k;
		for(k=-exponent;k<arraycontentnumber+1;k++){
			mantissa[index] = binaryfraction[k];
			index++;
		}		
	}
	if(sizeOfFloats == 2){
		int roundedmantissa[7];
		// condition possibilities
		char condition[20] ="placeholder";
		char halfwayup[20] = "halfwayup";
		char halfwaydown[20] = "halfwaydown";
		char halfway[20] = "halfway";
		char noround[20] = "noround";
		int count = 0;
		// Determining the mode of the round
		if(mantissasize > 7){
			int k;
			for(k=7;k<mantissasize;k++){
				if(mantissa[k] == 1  && mantissa[7] == 1){
					count++;
				}
			}
			if(count >1){
				strcpy(condition,halfwayup);
			}
			else if(count ==1){
				strcpy(condition,halfway);
			}
			else if(count == 0){
				strcpy(condition,halfwaydown);
			}
		}
		else{
			strcpy(condition,noround);
		}
		// Rounding mantissa halfwayup
		if(strcmp(condition,halfwayup) == 0){
			int k;
			int carry=1;
			for(k=6;k>=0;k--){
				roundedmantissa[k] = (mantissa[k] + carry) % 2;
				carry = (mantissa[k] + carry)/2;
			}
		}
		// noround - just adding zeros at the end of the mantissa
		if(strcmp(condition,noround) == 0){
			int k;
			for(k=0;k<7;k++){
				if(k<mantissasize){
					roundedmantissa[k] = mantissa[k];
				}
				else{
					roundedmantissa[k] = 0;
				}
			}
		}
		// Rounding mantissa halfwaydown
		if(strcmp(condition,halfwaydown) == 0){
			int k;
			for(k=0;k<7;k++){
				roundedmantissa[k] = mantissa[k];
			}
		}
		// Rounding mantissa halfway
		if(strcmp(condition,halfway) == 0){
			if(mantissa[6] == 1){
				int k;
				int carry=1;
				for(k=6;k>=0;k--){
					roundedmantissa[k] = (mantissa[k] + carry) % 2;
					carry = (mantissa[k] + carry)/2;
				}
			}
			else if(mantissa[6] == 0){
				int k;
				for(k=0;k<7;k++){
					roundedmantissa[k] = mantissa[k];
				}
			}
		}
		
		int m;
		int exponentindecimal = exponent + 127;
		int exponentinbinary[8];
		// Calculating exponent
		for(m=7;exponentindecimal>0;m--){
			exponentinbinary[m]=exponentindecimal%2;
			exponentindecimal=exponentindecimal/2;
		}
		if(m>=0){
			for(m;m>=0;m--){
				exponentinbinary[m] = 0;
			}
		}	
		int floatinbinary[16];
		// Adding 1 or 0 at the begginging according to sign.
		if(floating_number < 0){
			floatinbinary[0] = 1;
		}
		else if(floating_number > 0){
			floatinbinary[0] = 0;
		}
		// Combining exponent and roundedmantissa to form binary form the floating point number.
		int indexforexponentbinary = 0;
		for(m=1;m<9;m++){
			floatinbinary[m] = exponentinbinary[indexforexponentbinary];
			indexforexponentbinary++;
		}
		int indexforroundedmantissa = 0;
		for(m=9;m<=15;m++){
			floatinbinary[m] = roundedmantissa[indexforroundedmantissa];
			indexforroundedmantissa++;
		}
        int a;
	    for(a=0;a<16;a++)
		bin[a]=floatinbinary[a]+'0';
	}
	
	if(sizeOfFloats == 1){
		int roundedmantissa[4];
		// Condition possibilities
		char condition[20] ="placeholder";
		char halfwayup[20] = "halfwayup";
		char halfwaydown[20] = "halfwaydown";
		char halfway[20] = "halfway";
		char noround[20] = "noround";
		int count = 0;
		// Determining the mode of the round
		if(mantissasize > 4){
			int k;
			for(k=4;k<mantissasize;k++){
				if(mantissa[k] == 1  && mantissa[4] == 1){
					count++;
				}
			}
			if(count >1){
				strcpy(condition,halfwayup);
			}
			else if(count ==1){
				strcpy(condition,halfway);
			}
			else if(count == 0){
				strcpy(condition,halfwaydown);
			}
		}
		else{
			strcpy(condition,noround);
		}
		// Rounding mantissa halfwayup
		if(strcmp(condition,halfwayup) == 0){
			int k;
			int carry=1;
			for(k=3;k>=0;k--){
				roundedmantissa[k] = (mantissa[k] + carry) % 2;
				carry = (mantissa[k] + carry)/2;
			}
		}
		// noround just adding zeros at the end of the mantissa
		if(strcmp(condition,noround) == 0){
			int k;
			for(k=0;k<4;k++){
				if(k<mantissasize){
					roundedmantissa[k] = mantissa[k];
				}
				else{
					roundedmantissa[k] = 0;
				}
			}
		}
		// Rounding manttisa halfwaydown
		if(strcmp(condition,halfwaydown) == 0){
			int k;
			for(k=0;k<4;k++){
				roundedmantissa[k] = mantissa[k];
			}
		}
		// Rounding mantissa halfway
		if(strcmp(condition,halfway) == 0){
			if(mantissa[3] == 1){
				int k;
				int carry=1;
				for(k=3;k>=0;k--){
					roundedmantissa[k] = (mantissa[k] + carry) % 2;
					carry = (mantissa[k] + carry)/2;
				}
			}
			else if(mantissa[3] == 0){
				int k;
				for(k=0;k<4;k++){
					roundedmantissa[k] = mantissa[k];
				}
			}
		}		
		int m;	
		int exponentindecimal = exponent + 3;
		int exponentinbinary[3];
		// Calculating exponent
		for(m=2;exponentindecimal>0;m--){
			exponentinbinary[m]=exponentindecimal%2;
			exponentindecimal=exponentindecimal/2;
		}
		if(m>=0){
			for(m;m>=0;m--){
				exponentinbinary[m] = 0;
			}
		}
		int floatinbinary[8];
		// Adding 1 or 0 at the begginging according to sign.
		if(floating_number < 0){
			floatinbinary[0] = 1;
		}
		else if(floating_number > 0){
			floatinbinary[0] = 0;
		}
		// Combining exponent and roundedmantissa to form binary form the floating point number.
		int indexforexponentbinary = 0;
		for(m=1;m<4;m++){
			floatinbinary[m] = exponentinbinary[indexforexponentbinary];
			indexforexponentbinary++;
		}
		int indexforroundedmantissa = 0;
		for(m=4;m<=7;m++){
			floatinbinary[m] = roundedmantissa[indexforroundedmantissa];
			indexforroundedmantissa++;
		}
        int a;
	    for(a=0;a<8;a++)
		bin[a]=floatinbinary[a]+'0';
	}
	
	if(sizeOfFloats == 3){
		int roundedmantissa[12];
		// round mode possibilities
		char condition[20] ="placeholder";
		char halfway[20] = "halfway";
		char noround[20] = "noround";
		// Determining the mode of the round
		if(mantissasize > 12){
			strcpy(condition,halfway);
		}
		else{
			strcpy(condition,noround);
		}
		// noround just adding zeros at the end of the mantissa
		if(strcmp(condition,noround) == 0){
			int k;
			for(k=0;k<12;k++){
				if(k<mantissasize){
					roundedmantissa[k] = mantissa[k];
				}
				else{
					roundedmantissa[k] = 0;
				}
			}
		}
		// rounding mantissa halfway
		if(strcmp(condition,halfway) == 0){
			if(mantissa[11] == 1){
				int k;
				int carry=1;
				for(k=11;k>=0;k--){
					roundedmantissa[k] = (mantissa[k] + carry) % 2;
					carry = (mantissa[k] + carry)/2;
				}
			}
			else if(mantissa[11] == 0){
				int k;
				for(k=0;k<12;k++){
					roundedmantissa[k] = mantissa[k];
				}
			}
		}	
		int m;
		int exponentindecimal = exponent + 511;
		int exponentinbinary[10];
		// Calculating exponent
		for(m=9;exponentindecimal>0;m--){
			exponentinbinary[m]=exponentindecimal%2;
			exponentindecimal=exponentindecimal/2;
		}
		if(m>=0){
			for(m;m>=0;m--){
				exponentinbinary[m] = 0;
			}
		}
		int floatinbinary[24];
		// Adding 1 or 0 at the begginging according to sign.
		if(floating_number < 0){
			floatinbinary[0] = 1;
		}
		else if(floating_number > 0){
			floatinbinary[0] = 0;
		}
		int indexforexponentbinary = 0;
		// Combining exponent and roundedmantissa to form binary form the floating point number.
		for(m=1;m<11;m++){
			floatinbinary[m] = exponentinbinary[indexforexponentbinary];
			indexforexponentbinary++;
		}
		int indexforroundedmantissa = 0;
		for(m=11;m<=23;m++){
			if(m<=22){
				floatinbinary[m] = roundedmantissa[indexforroundedmantissa];
				indexforroundedmantissa++;	
			}
			else if(m >22){
				floatinbinary[m] = 0;
			}
		}
		int a;
	    for(a=0;a<24;a++)
		bin[a]=floatinbinary[a]+'0';
	}
	
	if(sizeOfFloats == 4){
		int roundedmantissa[12];
		// Round mode possibilities
		char condition[20] ="placeholder";
		char halfway[20] = "halfway";
		char noround[20] = "noround";
		// Determining the mode of the round
		if(mantissasize > 12){
			strcpy(condition,halfway);
		}
		else{
			strcpy(condition,noround);
		}
		// no round just adding zeros at the end of the mantissa
		if(strcmp(condition,noround) == 0){
			int k;
			for(k=0;k<12;k++){
				if(k<mantissasize){
					roundedmantissa[k] = mantissa[k];
				}
				else{
					roundedmantissa[k] = 0;
				}
			}
		}
		// Rounding mantisssa halfway
		if(strcmp(condition,halfway) == 0){
			if(mantissa[11] == 1){
				int k;
				int carry=1;
				for(k=11;k>=0;k--){
					roundedmantissa[k] = (mantissa[k] + carry) % 2;
					carry = (mantissa[k] + carry)/2;
				}
			}
			else if(mantissa[11] == 0){
				int k;
				for(k=0;k<12;k++){
					roundedmantissa[k] = mantissa[k];
				}
			}
		}
		int m;
		int exponentindecimal = exponent + 2047;
		int exponentinbinary[12];
		// Calculating exponent
		for(m=11;exponentindecimal>0;m--){
			exponentinbinary[m]=exponentindecimal%2;
			exponentindecimal=exponentindecimal/2;
		}
		if(m>=0){
			for(m;m>=0;m--){
				exponentinbinary[m] = 0;
			}
		}	
		int floatinbinary[32];
		// Adding 1 or 0 at the begginging according to sign.
		if(floating_number < 0){
			floatinbinary[0] = 1;
		}
		else if(floating_number > 0){
			floatinbinary[0] = 0;
		}
		int indexforexponentbinary = 0;
		// Combining exponent and roundedmantissa to form binary form the floating point number.
		for(m=1;m<13;m++){
			floatinbinary[m] = exponentinbinary[indexforexponentbinary];
			indexforexponentbinary++;
		}
		int indexforroundedmantissa = 0;
		for(m=13;m<=31;m++){
			if(m<=24){
				floatinbinary[m] = roundedmantissa[indexforroundedmantissa];
				indexforroundedmantissa++;
			}
			else if(m>24){
				floatinbinary[m] = 0;
			}
		}
		int a;
	    for(a=0;a<32;a++)
		bin[a]=floatinbinary[a]+'0';
	}
	return bin;
}

//The Function that finds and returns binary representations of unsigned integers.
char* Unsigned(int number,char bin[]){  
  char temp;
  int rmd;
  while(number!=0){
	rmd=number%2;
    number=number/2; 
    temp=rmd+'0';
    strncat(bin, &temp, 1); 
  }  
  char zero ='0';
  while(strlen(bin)<16){
  	strncat(bin, &zero, 1);
  } 
  strrev(bin);
  return bin;
}

//The Function that finds and returns binary representations of signed integers.
char *Signed(int number, char bin[]){
  if(number>=0)  // If the signed integer is positive, same sequence of code in Unsigned funciton can be executed.The result will be correct.
    return Unsigned(number,bin); 
  /*Else, number is multiplied by -1 and finds its two's complement representation then its complementary is taken and finally adds 1 to this
  complementary. With these steps the two's complement representation of actual number is found.*/
  else{  
    number=number*-1;
    char tempBin[17]="\0";
    strcpy(tempBin,Unsigned(number,bin));
    int i;
	for(i=0;i<16;i++){
	  if(tempBin[i]=='0')
	    tempBin[i]='1';
	  else
	    tempBin[i]='0';
	}
	for(i=15;i>=0;i--){
	  if(tempBin[i]=='0'){
	  	 tempBin[i]='1';
	  	 break;
	  }
	}
    for(i=i+1;i<16;i++)
      tempBin[i]='0';
    strcpy(bin,tempBin);
	return bin;  
  }
}

//This function takes a binary representation, finds its hexadecimal notation and gives us it according the given byte ordering type.
char* HexaNotation(char bin[],char hexaNotation[]){
  int i,j,bit,decimal;
  char hex;
  char subbin[5]="\0";
  for(i=0;i<=strlen(bin)-4;i+=4){
  	strncpy(subbin,bin+i,4);
	decimal=0;
    for(j=0;j<=3;j++){
      bit=subbin[j]-'0';
	  decimal+=bit*pow(2,3-j);     
	}
    if(decimal>=10)
	  hex='A'+decimal-10;
    else
      hex=decimal+'0';
    strncat(hexaNotation, &hex, 1);  
  }
  //This if function sorts the bytes of hexadecimal notation according the little endian.
  if(!strcasecmp(byteOrdType,"little endian")) {
  	char temp1,temp2;
	j =0;
    for(i=strlen(hexaNotation)-1;i>=5;i-=2){
      temp1 = hexaNotation[i];
      temp2= hexaNotation[i-1];
      hexaNotation[i-1]=hexaNotation[j];
      hexaNotation[i]=hexaNotation[j+1];
	  hexaNotation[j+1]=temp1; 
	  hexaNotation[j]=temp2;
	  j+=2;
	}
  }
  
  //The following parts are executed both little and big endian types. With these codes just space character is added among bytes.
  char temphexa[15] = "\0";
  char subhexa[5] = "\0";
  for(i=0;i<=strlen(hexaNotation)-2;i+=2){
    subhexa[0]=hexaNotation[i];
    subhexa[1]=hexaNotation[i+1];
    subhexa[2]=' ';
    strcat(temphexa,subhexa);
  }
  strcpy(hexaNotation,temphexa);
  return hexaNotation;
}

//Main function
int main() {  
  
  //Take byte ordering type
  char second[10]="\0";
  printf("Enter the byte ordering type (Little Endian, Big Endian) : ");
  scanf("%s",&byteOrdType);
  scanf("%s",&second);
  strcat(byteOrdType," ");
  strcat(byteOrdType,second);
  //Take the size of floating point data type.
  short sizeOfFloats;
  printf("Enter the size of floating point data type (1,2,3,4) : ");
  scanf("%hd",&sizeOfFloats);
  //Reading input file
  FILE *in_file = fopen("input.txt","r");
  //Writing output file
  FILE *out_file = fopen("output.txt","w");
  if(in_file==NULL)
	printf("File could not be opened!");
  if(out_file==NULL)
    printf("Unable to create file.");
  if(in_file!=NULL){
    //The variable that will hold the numbers as a string one by one.
	char number[21]="\0";
	char bin[35]="\0";
	char hexaNotation[20]="\0";
 	while (!feof(in_file)) { 
	  memset(bin,'\0',35);
	  memset(hexaNotation,'\0',20);
	  fgets(number,20,in_file);
	  //Control the number whether it is floating point number or not by checking '.' character inside it.
	  if (strchr(number,'.') != NULL) {
	  	long double nmbr = atof(number);
	  	fputs(HexaNotation(floating(nmbr,sizeOfFloats,bin),hexaNotation),out_file);
	  	fputs("\n",out_file);
	  }
	  //Control the number whether it is unsigned integer or not by checking 'u' character inside it.
	  else if (strchr(number,'u') != NULL) {
		unsigned short nmbr = atoi(number);
		fputs(HexaNotation(Unsigned(nmbr,bin),hexaNotation),out_file);
		fputs("\n",out_file);
	  }
	  // If the number is neither float nor unsigned, should be signed integer. 
	  else {	  	
	    short nmbr = atoi(number);
	    fputs(HexaNotation(Signed(nmbr,bin),hexaNotation),out_file);
	    fputs("\n",out_file);
	  }
   }
  }
  fclose(in_file);
  fclose(out_file);
return 0;
}
