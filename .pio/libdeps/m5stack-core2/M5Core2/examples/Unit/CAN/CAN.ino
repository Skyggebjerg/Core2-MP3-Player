/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/can
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/can
*
* Product: can.
* Date: 2021/9/1
*******************************************************************************
  Please connect to PORT-B,Press buttonA or buttonB to Send message if received message, screen will display
  请连接端口B,按钮A或按钮B发送消息，如果收到消息，屏幕将显示
*/

#include <M5Core2.h>
#include "ESP32CAN.h"
#include "CAN_config.h"

#define TX GPIO_NUM_26
#define RX GPIO_NUM_36

CAN_device_t CAN_cfg;  // CAN Config

int i = 0;

void setup() {
  M5.begin(true, false, true);
  M5.Lcd.drawString("CAN Unit Send&Received", 40, 3, 4);
  M5.Lcd.setCursor(0, 60, 4);

  CAN_cfg.speed = CAN_SPEED_125KBPS;  //Set the Can speed.  设置Can速度
  CAN_cfg.tx_pin_id = TX;             //Set the Pin foot.  设置引脚
  CAN_cfg.rx_pin_id = RX;

  CAN_cfg.rx_queue = xQueueCreate(10, sizeof(CAN_frame_t));

  ESP32Can.CANInit();  // Init CAN Module.  初始化Can
}

void loop() {
  CAN_frame_t rx_frame;

  if (M5.BtnA.wasPressed()) {
    rx_frame.FIR.B.FF = CAN_frame_std;
    rx_frame.MsgID = 1;
    rx_frame.FIR.B.DLC = 8;
    rx_frame.data.u8[0] = 'H';
    rx_frame.data.u8[1] = 'e';
    rx_frame.data.u8[2] = 'l';
    rx_frame.data.u8[3] = 'l';
    rx_frame.data.u8[4] = 'o';
    rx_frame.data.u8[5] = 'C';
    rx_frame.data.u8[6] = 'A';
    rx_frame.data.u8[7] = 'N';

    ESP32Can.CANWriteFrame(&rx_frame);
    M5.Lcd.println("Send Message1");
  }
  if (M5.BtnB.wasPressed()) {
    rx_frame.FIR.B.FF = CAN_frame_std;
    rx_frame.MsgID = 1;
    rx_frame.FIR.B.DLC = 8;
    rx_frame.data.u8[0] = 'M';
    rx_frame.data.u8[1] = '5';
    rx_frame.data.u8[2] = 'S';
    rx_frame.data.u8[3] = 'T';
    rx_frame.data.u8[4] = 'A';
    rx_frame.data.u8[5] = 'C';
    rx_frame.data.u8[6] = 'K';
    rx_frame.data.u8[7] = ' ';
    ESP32Can.CANWriteFrame(&rx_frame);
    M5.Lcd.println("Send Message2");
    Serial.println("B");
  }

  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) ==
      pdTRUE) {
    M5.Lcd.fillRect(0, 60, 320, 180, BLACK);
    M5.Lcd.setCursor(0, 60, 4);
    //do stuff!
    if (rx_frame.FIR.B.FF == CAN_frame_std) {
      printf("New standard frame");
      M5.Lcd.printf("New standard frame");
    } else {
      printf("New extended frame");
      M5.Lcd.printf("New extended frame");
    }
    if (rx_frame.FIR.B.RTR == CAN_RTR) {
      printf(" RTR from 0x%08x, DLC %d\r\n", rx_frame.MsgID,
             rx_frame.FIR.B.DLC);
      M5.Lcd.printf(" RTR from 0x%08x, DLC %d\r\n", rx_frame.MsgID,
                    rx_frame.FIR.B.DLC);
    } else {
      printf(" from 0x%08x, DLC %d\n", rx_frame.MsgID, rx_frame.FIR.B.DLC);
      M5.Lcd.printf(" from 0x%08x, DLC %d\r\n", rx_frame.MsgID,
                    rx_frame.FIR.B.DLC);
      for (int i = 0; i < 8; i++) {
        printf("%c\t", (char)rx_frame.data.u8[i]);
        M5.Lcd.printf("%c\t", (char)rx_frame.data.u8[i]);
      }
      printf("\n");
    }
  }

  M5.update();
  delay(200);
}