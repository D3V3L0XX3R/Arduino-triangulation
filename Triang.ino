#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Intel_Edison_BT_SPP.h>

Intel_Edison_BT_SPP spp = Intel_Edison_BT_SPP();

//SSID of your network 
char ssid[] = "HackUPC-Wifi";
//password of your WPA Network 
char pass[] = "wgBsdYcV";

//MACs from Nodes
byte bssid_1[6];
byte bssid_2[6];
byte bssid_3[6];

unsigned char bssid_11[6]="";//00:22:6B:F6:79:AF
unsigned char bssid_22[6]="";//C4:93:00:11:15:B0
unsigned char bssid_33[6]="";//E8:FC:AF:A6:0C:F2
char db1[2],db2[2],db3[2];


void setup() {
  Serial.begin(115200);
  Serial.println("Intel Edison BT/WiFi Triangulation!");
  spp.open();
  char COORDX_MAC1[]="10.385666";
  char COORDY_MAC1[]="1.854574";
  
  char COORDX_MAC2[]="12.209893";
  char COORDY_MAC2[]="13.936612";
  
  char COORDX_MAC3[]="12.209893";
  char COORDY_MAC3[]="13.936612";
}

void loop() {
  float ax, ay, a, bx, by, b, cx, cy, c, x, y;
  byte available_networks = WiFi.scanNetworks();
  for (int network = 0; network < available_networks; network++) {
    if (strcmp(WiFi.SSID(network), ssid) == 0) {
      long rssi=WiFi.RSSI(network);
      if(strcmp(WiFi.BSSID(bssid_1), bssid_11) == 0){
        db1=rssi;
      }else if(strcmp(WiFi.BSSID(bssid_2), bssid_22) == 0){
        db2=rssi;
      }else if(strcmpx(WiFi.BSSID(bssid_3), bssid_33) == 0){
        db3=rssi;
      }
    }
  }
  ax=atof(COORDX_MAC1);
  ay=atof(COORDY_MAC1);
  a=atof(db1);
  bx=atof(COORDX_MAC2);
  by=atof(COORDY_MAC2);
  b=atof(db2);
  cx=atof(COORDX_MAC3);
  cy=atof(COORDY_MAC3);
  c=atof(db3);
  bellaRoba(ax, ay, a, bx, by, b, cx, cy, c, &x, &y);
  send_bt_(x,y);
} 



void bellaRoba(float ax, float ay, float ad, float bx, float by, float bd, float cx, float cy, float cd, float *x, float *y){
  float minA, minB, minC, pA, pB, pC, e;
  *x = 0.f;
  *y = 0.f;
  minA = pow(*x - ax, 2) + pow(*y - ay, 2) - ad*ad;
  minB = pow(*x - bx, 2) + pow(*y - by, 2) - bd*bd;
  minC = pow(*x - cx, 2) + pow(*y - cy, 2) - cd*cd;

  do{
    e = abs(minA)/ad + abs(minB)/bd + abs(minC)/cd;

    *x += 1.0f;

    minA = pow(*x - ax, 2) + pow(*y - ay, 2) - ad*ad;
    minB = pow(*x - bx, 2) + pow(*y - by, 2) - bd*bd;
    minC = pow(*x - cx, 2) + pow(*y - cy, 2) - cd*cd;

  } while ((abs(minA)/ad + abs(minB)/bd + abs(minC)/cd) < e);
  *x -= 1.0f;
  minA = pow(*x - ax, 2) + pow(*y - ay, 2) - ad*ad;
  minB = pow(*x - bx, 2) + pow(*y - by, 2) - bd*bd;
  minC = pow(*x - cx, 2) + pow(*y - cy, 2) - cd*cd;
  do{
    e = abs(minA)/ad + abs(minB)/bd + abs(minC)/cd;

    *y += 1.0f;

    minA = pow(*x - ax, 2) + pow(*y - ay, 2) - ad*ad;
    minB = pow(*x - bx, 2) + pow(*y - by, 2) - bd*bd;
    minC = pow(*x - cx, 2) + pow(*y - cy, 2) - cd*cd;

  } while ((abs(minA)/ad + abs(minB)/bd + abs(minC)/cd) < e);
  *y -= 1.0f;
}


void send_bt(float x, float y) 
{
 ssize_t size = spp.write(x,y);

 if (size != -1) {
  Serial.println(x,y);
 }else{
  delay(500); 
 }
}
