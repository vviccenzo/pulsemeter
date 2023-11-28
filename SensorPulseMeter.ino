#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP_Mail_Client.h>

#include <HTTPClient.h>

#define REPORTING_PERIOD_MS 1000

// Credenciais do email de quem será enviado para envio de email
#define AUTHOR_EMAIL "AQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUI"
#define AUTHOR_PASSWORD "AQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUI"

// Credenciais do wifi para envio de email
#define WIFI_SSID "AQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUI"
#define WIFI_PASSWORD "AQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUI"

// Host do servidor que será enviado o email
#define SMTP_HOST "smtp.office365.com"

#define SMTP_PORT esp_mail_smtp_port_587

// Configurar acesso Wifi para envio de mensagem via Whats
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

Session_Config config;

SMTP_Message message;

// Token criado no app do Blynk
char auth[] = "w2yoZ3qIQpfq1DmTMYUcEB1Nd3s_HnwO";

// Acessos de wifi para conexão no app do Blynk
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

// Configuração para envio de mensagem via Whats
  // Token criado a partir da API do Meta Developers
String token="Bearer EAAT2I9snX70BOZCRHCe5JHPPPo8Svj9T9qwxTZBqNDHB7VVgZBblPYygukeqmmTV3CWnslhnheIh9pZCF8yrolCj0R1yVZAC5i74ZByvssnDDQsGqwZBkJGKkxG7OEC2gsFwq49vJ40ZBunZCnrQaNOaC4Be8UcKJZCHvF7hZCbtJWQsIHP1NfWq9SbgTSRrG8ZBqfG9ppFo9Xq1i8PflW7f6x8ZD";
  // Servidor que o Meta disponibiliza para hospedar o envio de mensagens
String servidor = "https://graph.facebook.com/v17.0/145056855366635/messages";
  // Corpo do Json que irá ser usado para enviar a mensagem
    // -> to: É o número é para quem será enviado a mensagem
    // -> name: É a mensagem em si que será enviada
String payload = "{ \"messaging_product\": \"whatsapp\", \"to\":\"5547996983831\", \"type\": \"template\", \"template\": { \"name\": \"Usuário da PulseMeter em risco!\", \"language\":{ \"code\": \"pt_BR\"}}}";
 
PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
  
void onBeatDetected() {
    Serial.println("Batida detectada!");
}
 
void setup() {
    Serial.begin(115200);
    
    pinMode(19, OUTPUT);
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Inicializando o oxímetro de pulso..");
 
    if (!pox.begin()) {
         Serial.println("FAILED");
         for(;;);
    }  else  {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    configEmailSend()
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}
 
void configEmailSend() {
    Serial.begin(115200);

#if defined(ARDUINO_ARCH_SAMD)
  while (!Serial)
    ;
  Serial.println();
  Serial.println("**** Custom built WiFiNINA firmware need to be installed.****\n");
  Serial.println("To install firmware, read the instruction here, https://github.com/mobizt/ESP-Mail-Client#install-custom-build-wifinina-firmware");
#endif

  Serial.println();

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  multi.addAP(WIFI_SSID, WIFI_PASSWORD);
  multi.run();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

  Serial.print("Connecting to Wi-Fi");

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    unsigned long ms = millis();
#endif

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    if (millis() - ms > 10000) {
      break;
    }
#endif
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  MailClient.networkReconnect(true);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  MailClient.clearAP();
  MailClient.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

  smtp.debug(1);

  smtp.callback(smtpCallback);

  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  config.login.user_domain = F("mydomain.net");

  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 0;
  config.time.day_light_offset = 0;
  config.time.timezone_env_string = "<-03>3"; 


  // Insere o cabeçalho do Email
  message.sender.name = F("Robótica para Todos");
  message.sender.email = AUTHOR_EMAIL;

  randomSeed(analogRead(0));
  
  // Inserir o assunto do Email
  String assunto =  "AJUDA! O portador do medidor de batimentos está em risco!";

  // Destinário padrão
  message.subject = assunto ;
  message.addRecipient(F("João"), "vvpcraft@gmail.com");
  
  // Inserir a mensagem do Corpo do Email
  String textMsg = "A mente que se abre a uma nova ideia jamais voltara ao seu tamanho original.'' Albert Einstein";
  
  message.text.content = textMsg;
  message.text.charSet = F("us-ascii");
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));
}

void sendEmail(){
    String assunto =  "AJUDA!";
    message.subject = assunto ;
    
    if (!smtp.connect(&config)) {
      ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
      return;
    }
  
    if (!smtp.isLoggedIn()) {
      Serial.println("\nNot yet logged in.");
    } else {
      if (smtp.isAuthenticated()){
        Serial.println("\nSuccessfully logged in.");
      } else {
        Serial.println("\nConnected with no Auth.");
      }
    }
  
    
    if (!MailClient.sendMail(&smtp, &message)) {
      ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    }
}

void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());

  if (status.success()) {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
      SMTP_Result result = smtp.sendingResult.getItem(i);
      
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
  
    Serial.println("----------------\n");

    smtp.sendingResult.clear();
}

void sendMessageWhats() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(servidor.c_str());
    http.addHeader("Content-Type", "application/json"); 
    http.addHeader("Authorization", token);    
    int httpPostCode = http.POST(payload);
    if (httpPostCode > 0) {
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
    }

    http.end();
  } else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  pox.update();
  Blynk.run();
 
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      Serial.print("Heart rate:");
      Serial.print(BPM);
      Serial.print(" bpm / SpO2:");
      Serial.print(SpO2);
      Serial.println(" %");
 
      if(BPM > 100) {
        sendEmail()
        sendMessageWhats()
      }

      Blynk.virtualWrite(V3, BPM);
      Blynk.virtualWrite(V4, SpO2);
 
      tsLastReport = millis();
  }
}