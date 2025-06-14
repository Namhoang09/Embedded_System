#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "time.h"

// Cấu hình UART
#define RXD2 16  // RX ESP32 (nối TX STM32)

// WiFi
const char* ssid = "P201";
const char* password = "21212121";

// Gmail
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "hnam910204@gmail.com"
#define AUTHOR_PASSWORD "ghfd lllo gcgb evyn"

SMTPSession smtp;
SMTP_Message message;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Đang kết nối WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi đã kết nối");

  // Cấu hình NTP (GMT+7)
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Đang lấy thời gian thực...");
    delay(1000);
  }

  smtp.debug(0); // Bật log nếu cần
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();

    Serial.println("Nhận từ STM32: " + data);

    if (data.startsWith("GAS DETECTED")) {
      int commaIndex = data.indexOf(',');
      String ppm = "???";

      if (commaIndex != -1) {
        ppm = data.substring(commaIndex + 1);
      }

      // Lấy thời gian thực
      struct tm timeinfo;
      if (getLocalTime(&timeinfo)) {
        char timeStr[30];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

        // Tạo nội dung gửi
        String subject = "CẢNH BÁO KHÍ GAS";
        String content = "GAS DETECTED at " + String(timeStr) +
                         "\nNồng độ khí gas vượt ngưỡng an toàn! PPM = " + ppm;

        sendEmail(subject, content);
      } else {
        Serial.println("Không lấy được thời gian!");
      }
    }
  }
}

void sendEmail(String subject, String content) {
  ESP_Mail_Session session;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  message.sender.name = "ESP32 Gas Monitor";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = subject;
  message.clearRecipients();
  message.addRecipient("Người nhận", "namde247@gmail.com");

  message.text.content = content;
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session)) {
    Serial.println("Không thể kết nối SMTP!");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Gửi email thất bại: " + smtp.errorReason());
  } else {
    Serial.println("Email đã gửi thành công!");
  }

  smtp.closeSession();
}
