#include <OneWire.h>// OneWire kütüphanesini ekliyoruz.
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sıcaklık sensörünü bağladığımız dijital pini 2 olarak belirliyoruz.
int DS18B20_Pin = 2; 

// Sıcaklık Sensörü Giriş-Çıkışı
OneWire ds(DS18B20_Pin);  // 2. Dijital pinde.

const int trig = 12;
const int echo = 11;

int sure = 0,mesafe = 0;
float hiz = 0,degisken = 0;

const int uyari_led_kirmizi = 8;
const int uyari_led_sari = 9;
const int uyari_led_yesil = 10;
const int uyari_ses = 7;

void setup() {
   pinMode(trig,OUTPUT);
   pinMode(echo,INPUT);
   
   pinMode(uyari_led_kirmizi, OUTPUT);
   pinMode(uyari_led_sari, OUTPUT);
   pinMode(uyari_led_yesil, OUTPUT);
   pinMode(uyari_ses, OUTPUT);
   
   lcd.begin(16,2);
   Serial.begin(9600); // Seri iletişimi başlatıyoruz.
   
}

void loop() {
  // temperature değişkenini sıcaklık değerini alma fonksiyonuna bağlıyoruz.
  float temperature = getTemp();

  
  digitalWrite(trig, HIGH);//ses dalgası uretme
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);

  hiz = (331.4 * sqrt(1+(temperature/273)))/10000;
  degisken = 1/hiz;
  sure = pulseIn(echo, HIGH);//echo pinini takip eder degiskene atar
  mesafe = (sure/2)/degisken;
  
  
  
  
  if(temperature>=10 && temperature<24)
      {if(mesafe>=5 && mesafe<15)
          {digitalWrite(uyari_led_kirmizi, HIGH);
          digitalWrite(uyari_led_sari, LOW);
          digitalWrite(uyari_led_yesil, LOW);
          digitalWrite(uyari_ses, HIGH);
          delay(50);
          digitalWrite(uyari_ses, LOW);
          delay(50);}
       if(mesafe>=15 && mesafe<25)
          {digitalWrite(uyari_led_kirmizi, LOW);
          digitalWrite(uyari_led_sari, HIGH);
          digitalWrite(uyari_led_yesil, LOW);
          digitalWrite(uyari_ses, HIGH);
          delay(100);
          digitalWrite(uyari_ses, LOW);
          delay(100);}   
       else
          {digitalWrite(uyari_led_kirmizi, LOW);
          digitalWrite(uyari_led_sari,  LOW);
          digitalWrite(uyari_led_yesil, HIGH);}}
  else if(temperature>=24 && temperature<30)        
      {if(mesafe>=10 && mesafe<20)
          {digitalWrite(uyari_led_kirmizi, HIGH);
          digitalWrite(uyari_led_sari, LOW);
          digitalWrite(uyari_led_yesil, LOW);
          digitalWrite(uyari_ses, HIGH);
          delay(50);
          digitalWrite(uyari_ses, LOW);
          delay(50);}
       if(mesafe>=20 && mesafe<30)
          {digitalWrite(uyari_led_kirmizi, LOW);
          digitalWrite(uyari_led_sari, HIGH);
          digitalWrite(uyari_led_yesil, LOW);
          digitalWrite(uyari_ses, HIGH);
          delay(100);
          digitalWrite(uyari_ses, LOW);
          delay(100);}   
       else
          {digitalWrite(uyari_led_kirmizi, LOW);
          digitalWrite(uyari_led_sari,  LOW);
          digitalWrite(uyari_led_yesil, HIGH);}}   
  else if(temperature>=30 && temperature<45)        
      {if(mesafe>=15 && mesafe<25)
          {digitalWrite(uyari_led_kirmizi, HIGH);
          digitalWrite(uyari_led_sari, LOW);
          digitalWrite(uyari_led_yesil, LOW);
          digitalWrite(uyari_ses, HIGH);
          delay(50);
          digitalWrite(uyari_ses, LOW);
          delay(50);}
       if(mesafe>=25 && mesafe<35)
          {digitalWrite(uyari_led_kirmizi, LOW);
          digitalWrite(uyari_led_sari, HIGH);
          digitalWrite(uyari_led_yesil, LOW);
          digitalWrite(uyari_ses, HIGH);
          delay(100);
          digitalWrite(uyari_ses, LOW);
          delay(100);}   
       else
          {digitalWrite(uyari_led_kirmizi, LOW);
          digitalWrite(uyari_led_sari,  LOW);
          digitalWrite(uyari_led_yesil, HIGH);}}
  
  Serial.print("Sicaklik: ");
  Serial.println(degisken);
  Serial.print("Mesafe: ");
  Serial.println(mesafe);
  
  lcd.clear();
  lcd.home();
  lcd.print("Sicaklik=");
  lcd.setCursor(10,0);
  lcd.print(temperature);
  lcd.setCursor(0,1);
  lcd.print("Mesafe=");
  lcd.setCursor(10,1);
  lcd.print(mesafe);
  // 1 saniye bekliyoruz. Monitörde saniyede 1 sıcaklık değeri yazmaya devam edecek.
  delay(500);
}

// Aşağıdaki fonksiyon DS18B20 sıcaklık sensörümüzden gelen verileri
// Celcius cinsinden sıcaklık değerlerine çevirmek için kullanılıyor.
// Herhangi bir değişiklik yapmamız gerekmiyor.

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;



}
