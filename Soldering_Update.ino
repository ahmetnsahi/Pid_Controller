#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PID_v1.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 

int pwm=6;
int  buzzer=11; 

//pid paremetreleri
double Setpoint, Input, Output;
double Kp=3, Ki=6, Kd=18;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int WindowSize=500;

int Win=750;
unsigned long windowStartTime;

//milis fonk tanımları
unsigned long eskiZaman=0, ez=0 ;
unsigned long yeniZaman , yz;

  const int okumaSayisi = 12;
  int okumalar[okumaSayisi];
  int indeks = 0;
  int toplam = 0;
  int ortalama = 0;

//Varsayılan set sıcaklıgı
int set=330;  //SET degerimiz
int temp=0;
unsigned long slp=600000;
//buton tanımları
#define b1 7
#define b2 10
#define b3 9
#define b4 8
const int Geri = 8;
const int Ileri = 9;
const int Onay = 10;
const int Cik = 7;
int menu = 1;
int smenu=1;
uint8_t ses=50;     //max 255 degeri alsın diye uint tanımlandı
int uyku=10;  //dakika 



byte power[8]={0x2,0x4,0xC,0x1F,0x6,0x4,0x8,0x0}; //power simgesi 


int t=analogRead(A0);
int volt=analogRead(A3);



void setup() {
pinMode(Ileri, INPUT);
pinMode(Geri, INPUT);
pinMode(Onay, INPUT);
pinMode(Cik, INPUT);
pinMode(buzzer,OUTPUT);
analogWrite(pwm,0 );
 
int t=analogRead(A0);
int volt=analogRead(A3);

lcd.begin();

lcd.createChar(0, power); 
lcd.write(byte(0));


 test(t);
                                           // pwm cıkışı sıfırlandı  Test yaptı havya saglamsa alta inder.....
 //-----------------------------------------------------------------------------------------------------------------------//

 
 Serial.begin(9600);
 windowStartTime = millis();// zaman başalattı
 Setpoint = 0;
 myPID.SetOutputLimits(0, 3); //cıkış pwm 
 myPID.SetMode(AUTOMATIC);//pıd açtı 
// myPID.SetTunings(Kp,Ki,Kd);


  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Set: ");






  



}








void test(int t){
  
 while(1){
         if(t>900){

              analogWrite(pwm,0 );
             lcd.setCursor(0,0);
             lcd.print("HAVYAYI KONTROL");
             lcd.setCursor(0,1);
             lcd.print("ET VE RESETLE!!!");
            
             
            
             
             digitalWrite(buzzer,HIGH);
             delay(500);
             digitalWrite(buzzer,LOW);
             delay(200);
             lcd.clear();
             digitalWrite(buzzer,HIGH);
             delay(250);  
              digitalWrite(buzzer,LOW);
             delay(100);
           
             lcd.clear();   
             analogWrite(pwm,0 );
 
 }
  

else{
             lcd.setCursor(3,0);
             lcd.print("HOSGELDINIZ");
             lcd.setCursor(5,1);
             lcd.print("WELCOME");
             analogWrite(pwm,0 );
             delay(2000);
             lcd.clear();
             break;
}

}

}


int term (){
     int to=ort();
     temp=map(to,0,1023,0,520);
     return temp;
}

int tpid(){
     t=analogRead(A0);
     temp=map(t,0,1023,0,510);
     return temp;
}


int ort(){
  
   
    //tanımlanan dizinin indeks pozisyonunda duran degeri çıkar
    toplam = toplam - okumalar[indeks];
    //onun yerine yeni bir ölçüm al ve kaydet
    okumalar[indeks] = tpid();
    //toplama yeni ölçümü ekle
    toplam = toplam + okumalar[indeks];
    //dizinin farklı bir pozisyonuna ilerle
    indeks++;
    //dizinin boyutunu berirlemek için okumaSayisi'nı sınırla
    if (indeks == okumaSayisi) indeks = 0;
    //dizinin içindeki değerlerin ortalamasını al
    ortalama = toplam / (okumaSayisi-1);
    return ortalama;
  
  
  }

int volts (){
     t=analogRead(A3);
     volt=map(t,0,1023,0,28);
     return volt;
   
     }
     


void buzr(){
analogWrite(buzzer,ses);
delay(200);
digitalWrite(buzzer,LOW); 
 
}




void sleep(){
  
      
       digitalWrite(buzzer,HIGH);
       delay(300);
       digitalWrite(buzzer,LOW);
       delay(200);
       digitalWrite(buzzer,HIGH);
       delay(800);
       digitalWrite(buzzer,LOW);
       delay(1);
       
       while(digitalRead(Onay)==0){
       lcd.clear();
       analogWrite(pwm,0 );
       delay(0);
       analogWrite(pwm,0 );
       lcd.setCursor(3,0);
       lcd.print("Sleep MODE");
       delay(600);  
             
 }
}













void loop() {



 while(1){
 
 ses=constrain(ses,0,90);
 set= constrain(set,0,475) ; //sınır belirledi max sıcaklık 

 
 yz=millis(); 
 if(yz-ez >uyku*60000){
 sleep();
 ez = yz; 
 }

 
 yeniZaman = millis();


 

 if(yeniZaman-eskiZaman > 500
 ){

     lcd.setCursor(5, 0);
     lcd.print("   ");
     lcd.setCursor(5, 0);
     lcd.print(tpid());
     lcd.setCursor(12, 0);
     lcd.print("    ");
     lcd.setCursor(13, 0);
     lcd.print(volts());
     lcd.setCursor(15, 0);
     lcd.print("V");

     /* Eski zaman değeri yeni zaman değeri ile güncelleniyor */
     eskiZaman = yeniZaman;
  }


lcd.setCursor(5, 1);
lcd.print(set);

Setpoint = set; //set sıcaklık pid  girildi 
Input = tpid();
myPID.Compute();
 
if (Output==3){
    
 analogWrite(pwm,255);
 }
 else if (Output==2){
    
 analogWrite(pwm,200);
 }
 else{
  analogWrite(pwm,0);
 }
 
 
 
 if(Output>0){
  lcd.setCursor(15,1);
  lcd.write(byte(0));
 }
 else{
  lcd.setCursor(15,1);
  lcd.print(" ");
 }

 
 
 //Start PID code
 

  
 

        if(tpid()>485){ //HATA DURUMUNDA UYARIYA GEÇET
            
            while(1){
             lcd.setCursor(0,0);
             lcd.print("HAVYAYI KONTROL");
             lcd.setCursor(0,1);
             delay(100);
             lcd.print("ET VE RESETLE!!!");
             digitalWrite(buzzer,HIGH);
             delay(400);
             digitalWrite(buzzer,LOW);
             delay(200);
             digitalWrite(buzzer,HIGH);
             delay(200);  
              digitalWrite(buzzer,LOW);
             delay(100);
             
               
             lcd.clear();
             lcd.setCursor(3,0);
             lcd.print("TEHLIKE!");             
             delay(100);
             lcd.setCursor(3,1);
             lcd.print("DANGEROUS!");           
             digitalWrite(buzzer,HIGH);
             delay(400);
             digitalWrite(buzzer,LOW);
             delay(200);
             digitalWrite(buzzer,HIGH);
             delay(200);  
              digitalWrite(buzzer,LOW);
             delay(100);
             
              
         
             analogWrite(pwm,0 );
             lcd.clear();
             analogWrite(pwm,0 );}
             
             }
  




 if(digitalRead(b1) ==1){
   buzr();} 
  
 if(digitalRead(b2) ==1)
  { buzr();   
  
     while(digitalRead(Cik)==0){
       analogWrite(pwm,0);
       anaMenu();
       
      
      }
      lcd.clear();
      delay (10);
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.setCursor(0, 1);
      lcd.print("Set: ");
    
       
  }
  if(digitalRead(b3) ==1){
 set--;   buzr(); 
 }

   if(digitalRead(b4) ==1){
 set++;    buzr();  }




Serial.print(Input);
Serial.print(" ");
Serial.print(Output);
Serial.print(" ");
Serial.println(Setpoint);
//Serial.println(Setpoint);

}
}


void anaMenu(){
     
      lcd.clear();
      lcd.print(">Hafiza Modlari");
      lcd.setCursor(0, 1);
      lcd.print(" Uyku ayarlari");
 while(digitalRead(b1)==0){   
    while (digitalRead(Ileri)){
    menu++;
    updateMenu();
    buzr();
     
    ;
  }
  while (digitalRead(Geri)){
    menu--;
    updateMenu();
    buzr();
    
   ;
  }
  while (digitalRead(Onay)){
    executeAction();
    updateMenu();
    buzr();
 
   
  }
 }
  
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 5;
    
       break;
    case 1:
      lcd.clear();
      lcd.print(">Hafiza Modlari");
      lcd.setCursor(0, 1);
      lcd.print(" Uyku ayarlari");
       
      break;
    case 2:
      lcd.clear();
       lcd.print(" Hafiza Modlari");
      lcd.setCursor(0, 1);
      lcd.print(">Uyku ayarlari");
       
      break;
    case 3:
      lcd.clear();
      lcd.print(">Buzzer ayarlari");
      lcd.setCursor(0, 1);
      lcd.print(" Gelistirici");
      
      break;
    case 4:
      lcd.clear();
      lcd.print(" Buzzer ayarlari");
      lcd.setCursor(0, 1);
      lcd.print(">Gelistirici");
     
      break;
    case 5:
     
      menu = 0;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      menu=1;
      break;
      
  }
}



void sanaMenu(){

     lcd.clear();
     lcd.print(">350");
     lcd.setCursor(0, 1);
     lcd.print(" 375"); 
     delay(1000);
while(digitalRead(Cik)==0){
    
    while (digitalRead(Ileri)){
    smenu++;
    supdateMenu();
    buzr();
   
    ;
  }
  while (digitalRead(Geri)){
    smenu--;
    supdateMenu();
    buzr();
  
   ;
  }
 
  while (digitalRead(Onay)){
    sAction();
    supdateMenu();
    buzr();
   
   
  }}
  
}



void supdateMenu() {
  switch (smenu) {
    case 0:
      smenu = 5;
       break;
    case 1:
      lcd.clear();
      lcd.print(">350");
      lcd.setCursor(0, 1);
      lcd.print(" 375");
      break;
    case 2:
      lcd.clear();
       lcd.print(" 350");
      lcd.setCursor(0, 1);
      lcd.print(">375");
      break;
    case 3:
      lcd.clear();
      lcd.print(">400");
      lcd.setCursor(0, 1);
      lcd.print(" 450");
      break;
    case 4:
      lcd.clear();
      lcd.print(" 400");
      lcd.setCursor(0, 1);
      lcd.print(">450");
      break;
    case 5:
      smenu = 0;
      break;
  }
}








void sAction() {
  switch (smenu) {
    case 1:
      saction1();
      break;
    case 2:
      saction2();
      break;
    case 3:
      saction3();
      break;
    case 4:
      saction4();
      smenu=1;
      break;
      
  }
}
















void action1() {
   
   
   while(digitalRead(Cik)==0){
        sanaMenu();
      
      
      }
       
   
      

}




  




void action2() {
 
   lcd.clear();
  while(digitalRead(Cik)==0){
    uyku= constrain(uyku,0,20);
    lcd.setCursor(1,0);
    lcd.print("Uykuya Gecirme");
    lcd.setCursor(5,1);
    lcd.print("Dk:");

   
    while (digitalRead(Ileri)){
    uyku--;  
    buzr();
    
    
  }
  while (digitalRead(Geri)){
    uyku++;
    buzr();
    
    
   
  }
  lcd.setCursor(8,1);
  lcd.print("   ");
  lcd.setCursor(8,1);
  lcd.print(uyku);
  
}
}



void  action3() {
  lcd.clear();
  while(digitalRead(Cik)==0){
    ses=constrain(ses,0,90);
    lcd.setCursor(3,0);
    lcd.print("Ses sevyesi");  
    while (digitalRead(Ileri)){
     ses=ses-5;       
     buzr();   
     }
    while (digitalRead(Geri)){
     
     ses=ses+5;
     buzr();
     }     
  
    
    lcd.setCursor(7,1);
    lcd.print("    ");
    lcd.setCursor(7,1);
    lcd.print(ses);
    
    
  }
  
}


void action4() { 

  while(digitalRead(Cik)==0){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Made in TURKEY");
  lcd.setCursor(2, 1);
  lcd.print(">Ahmet Sahin<");
  delay(333);}

}


void saction1() {
 
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("350C ayarlandi");
  set=350;
  delay(800);
  
  
}
  
  

void saction2() {
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("375 C ayarlandi");
  set=375;
  delay(800);
  
}
  


void saction3() {
 
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("400 C ayarlandi");
  set=400;
  delay(800);
  
}


void saction4() {
 
 
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("450 C ayarlandi");
  set=450;
  delay(800);
  
}

























 
  

  
 



 



   



  
 
