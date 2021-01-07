

/******************************************************************************
 *  NM500 NeuroShield Board SimpleScript
 *  Simple Test Script to understand how the neurons learn and recognize
 *  revision 1.1.3, 01/03, 2018
 *  Copyright (c) 2017 nepes inc.
 *  
 *  Please use the NeuroShield library v1.1.3 or later.
 *  https://github.com/nepes-ai/neuroshield 
 ******************************************************************************/

#include <NeuroShield.h>
#include <SdFat.h>


#define NM500_SPI_SS 7

#define VECTOR_LENGTH 256
#define READ_COUNT 3

NeuroShield hnn;

uint8_t vector[VECTOR_LENGTH];
uint16_t response_nbr;
int i;
int cat_num=1;
uint8_t minus=0;
int timee=0;
int ncount=0;
int j=0;
uint16_t ncr, aif, cat;
uint16_t model[NEURON_SIZE];
long sum=0;
uint16_t dists[READ_COUNT], cats[READ_COUNT], nids[READ_COUNT];




void setup() {
  
bool SD_detected=false;
SdFat SDD;
char empty;
char num[3];
uint8_t nu[160];
uint16_t k=0;
uint16_t m=0;
//int nn[4][2]={{300,150},{1152,576},{1920,960},{2416,1218}};
//int nn[4][2]={{384,192},{1152,576},{1920,960},{2416,1218}};
int nn[4][2]={{384,192},{1152,576},{1920,960},{2168,1044}};
//int nn[4][2]={{384,384},{1152,1218},{1920,1218},{2416,1218}};
//int nn[4][2]={{192,96},{576,288},{960,480},{1208,609}};
//int nn[4][2]={{10,10},{576,288},{960,480},{1208,609}};
//int nn[4][2]={{192,192},{576,576},{960,960},{1208,1208}};
//int nn[4][2]={{144,144},{576,288},{960,480},{1208,609}};
int result;
uint16_t ok=0;
uint16_t ran=0;
int total=0;
float per=0;
uint16_t dist,cate,nid;
int degen=0;
int pos=0,neg=0;
int mm=0;
uint8_t incr=1;
uint8_t incr2=2;
int temp=0;
int temp2=0;
int tf=0;
int ts=0;
 
  /*
  Serial.begin(9600);
  while (!Serial);    // wait for the serial port to open
  if (hnn.begin(NM500_SPI_SS) != 0){ Serial.print("\nThere are "); Serial.print(hnn.total_neurons); Serial.print(" neurons\n");}
  else {Serial.print("\n NOT properly connected!"); while(1);}
*/
 Serial.begin(9600);


 hnn.begin(NM500_SPI_SS);


  // if you want to run in lsup mode, uncomment below
  //norm_lsup = 0x80;
   
  hnn.setGcr(1);
  hnn.forget();

    
Serial.println("start");
Serial.print(":nsr:");Serial.println(hnn.getNsr()); 
  /*
  minus=hnn.total_neurons/576;
  minus=!!(minus-1);
*/
  minus=hnn.total_neurons/576;
  minus/=2;
  
 

//sum=hnn.readcard("test.txt");

//Serial.println(sum);
if (!SD_detected) {
    SD_detected = SDD.begin(ARDUINO_SD_CS);
  }

  // SD card not found
//hnn.setMinif(15000);
//hnn.setMaxif(10000);



  File SDfile2 = SDD.open("train993.txt",O_READ);
File SDfile = SDD.open("train883.txt",O_READ);

  if (!SDfile and !SDfile2) {
    Serial.print("open fail");
  }

//learn_negative
pos=nn[minus][0]-1;
neg=nn[minus][1];
//temp=pos/2;
//temp=10;
//temp2=50;

//Serial.print(temp);



 
  while(pos!=0 or neg!=0)
  {
    //k=random(1,1000);
    //k=k%5;


              
    //random(20,24)
    
    
    //if(k==1 or k==2)
    for(m=0;m<34;m++)
    {
   
      if(neg!=0)
    {
     for(i=0;i<160;i++){
    num[0]=SDfile2.read();
    num[1]=SDfile2.read();
    num[2]=SDfile2.read();
    empty=SDfile2.read();
    nu[i]=atoi(num);
   // if(i==0) Serial.print(nu[i]);
    //nu[i]=min(255,2*atoi(num));
  }
  //if(neg%temp2==0) incr2+=2;

    hnn.learn(nu,160,2);
    empty=SDfile2.read();
    empty=SDfile2.read();
    neg--;
    }
    }
    
    //if(k==0)
    for(mm=0;mm<1;mm++)
    {
     
      if(pos!=0)
      {
    
      for(i=0;i<160;i++)
  {
    num[0]=SDfile.read();
    num[1]=SDfile.read();
    num[2]=SDfile.read();
    empty=SDfile.read();
    
   nu[i]=atoi(num);
   //if(i==0) Serial.print(nu[i]);
   //nu[i]=min(255,2*atoi(num));

  }
  //if(pos==temp){ incr+=2;}

  hnn.learn(nu,160,1);
  empty=SDfile.read();
  empty=SDfile.read();
  pos--;
      }}
    

    
//random(1,3)

  }

  SDfile.close();
  SDfile2.close();
  
Serial.println(hnn.getNcount()); 
Serial.println();






ncount=hnn.getNcount();

for (i = 1; i <= ncount; i++) {
      hnn.readNeuron(i, model, &ncr, &aif, &cat);
      //Serial.print("\nneuron#"); Serial.print(i); Serial.print("\tmodel=");
      for (j = 0; j < VECTOR_LENGTH; j++) {
        //Serial.print(model[j]); Serial.print(", ");
      }
      //Serial.print("model[0]:");Serial.print(model[0]);
      //Serial.print("\tncr="); Serial.print(ncr);  
      //Serial.print("\taif="); Serial.print(aif);     
      //Serial.print("\tcat="); Serial.println(cat & 0x7FFF); if (cat & 0x8000){ Serial.println(" (degenerated)");}
      if(cat==1) tf++;
      else if(cat==2) ts++;
  }
  
Serial.print("tf:");Serial.println(tf);
Serial.print("ts:");Serial.println(ts);

//Serial.print("maxif");Serial.println(hnn.getMaxif());
//Serial.print("minif");Serial.println(hnn.getMinif());
tf=0;
ts=0;
  
  /*
  for(i=1;i<=ncount;i++){
    hnn.readNeuron(i,model,&ncr,&aif,&cat);
    if(cat&0x8000) degen+=1;
  }

  Serial.print(degen);
  */
  
File SDfile3 = SDD.open("test3.txt",O_READ);
  for(k=0;k<1132;k++)
  {
  for(i=0;i<160;i++)
  {
    num[0]=SDfile3.read();
    num[1]=SDfile3.read();
    num[2]=SDfile3.read();
    empty=SDfile3.read();
    nu[i]=atoi(num);
 // if(i==0) Serial.print(nu[i]);
    
  }
  //m=micros();
  result=hnn.classify(nu,160,&dist,&cate,&nid);

  //Serial.print("dist:");Serial.println(dist);
  //Serial.print("cate:");Serial.println(cate);
  //m=micros()-m;
  empty=SDfile3.read();

  if(result!=0 and cate==1) {ok++; tf++;}
  total++;
  }

//per=(float)ok/(float)total*100;
//Serial.print("per:");Serial.println(per);

SDfile3.close();





File SDfile4 = SDD.open("test4.txt",O_READ);
  for(k=0;k<453;k++)
  {
  for(i=0;i<160;i++)
  {
    num[0]=SDfile4.read();
    num[1]=SDfile4.read();
    num[2]=SDfile4.read();
    empty=SDfile4.read();
    nu[i]=atoi(num);
  //if(i==0) Serial.print(nu[i]);
    
  }
  //m=micros();
  result=hnn.classify(nu,160,&dist,&cate,&nid);

  //Serial.print("dist:");Serial.println(dist);
  //Serial.print("cate:");Serial.println(cate);
  //m=micros()-m;
  empty=SDfile4.read();

  if(result!=0 and cate==2){ ok++; ts++;}
  total++;
  }

per=(float)ok/(float)total*100;
Serial.print("per:");Serial.println(per);
Serial.print("tf:");Serial.println(tf);
Serial.print("ts:");Serial.println(ts);
SDfile4.close();


}

void loop()
{
 
}
