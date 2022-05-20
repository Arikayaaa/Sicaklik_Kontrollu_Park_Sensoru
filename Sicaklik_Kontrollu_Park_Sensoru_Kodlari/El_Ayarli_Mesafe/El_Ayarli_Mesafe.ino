#include <OneWire.h>// OneWire kütüphanesini ekliyoruz.
#include<LiquidCrystal.h>
LiquidCrystal lcd(8,7,6,5,4,3);

// Sıcaklık sensörünü bağladığımız dijital pini 2 olarak belirliyoruz.
int DS18B20_Pin = 2; 

// Sıcaklık Sensörü Giriş-Çıkışı
OneWire ds(DS18B20_Pin);  // 2. Dijital pinde.

const int trig = 12;
const int echo = 13;

int sure = 0,mesafe = 0;

const int uyari = 11;

void setup() {
   pinMode(trig,OUTPUT);
   pinMode(echo,INPUT);
   
   pinMode(uyari, OUTPUT);
   
   lcd.begin(16,2);
   Serial.begin(9600); // Seri iletişimi başlatıyoruz.
   
}

void loop() {
  // temperature değişkenini sıcaklık değerini alma fonksiyonuna bağlıyoruz.
  float temperature = getTemp();

  
  digitalWrite(trig, HIGH);//ses dalgası uretme
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);

  
  sure = pulseIn(echo, HIGH);//echo pinini takip eder degiskene atar
  mesafe = (sure/2)/28.9;
  
  
  
  
  if(temperature>=10 && temperature<23)
      {if(mesafe>=5 && mesafe<15)
          digitalWrite(uyari, HIGH);
       else
          digitalWrite(uyari, LOW);}
  else if(temperature>=23 && temperature<30)        
      {if(mesafe>=15 && mesafe<25)
          digitalWrite(uyari, HIGH);
       else
          digitalWrite(uyari, LOW);}    
  else if(temperature>=30 && temperature<45)        
      {if(mesafe>=25 && mesafe<35)
          digitalWrite(uyari, HIGH);
       else
          digitalWrite(uyari, LOW);}    
  
  
  
  
  Serial.print("Sicaklik: ");
  Serial.println(temperature);
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
  delay(1000);
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



