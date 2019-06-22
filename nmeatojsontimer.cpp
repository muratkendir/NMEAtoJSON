#include "nmeatojsontimer.h"
#include <QDebug>

/*İlk kodlar*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*İlk kodlar son*/

/*Burayı ben ekledim*/
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
/*Burayı ben ekledim*/

#define MAX_LONGITUDE 180
#define MAX_LATITUDE 90
#define KNOTS_TO_KM 1.852
#define DELIMITER__ ","
/* Change this value to the delimiter of your
                              SourceFile*/

/* Function prototypes*/
void tokenize( char *oneline, FILE *fileName);
float convertFromNmeaSentenceToDecimalCoord(float coordinates, const char *val);
float convertFromKnotsToKmHour(float knots);

std::string getLastLine(std::ifstream& in)
{
   std::string line;
   while (in >> std::ws && std::getline(in, line)) // skip empty lines
       ;

   return line;
}


MyTimer::MyTimer()
{
    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(MyTimerSlot()));

    // msec
    timer->start(1000);
}

void MyTimer::MyTimerSlot()
{

    MyTimer w;
    /*Burayı ben ekledim Başlangıç*/
    std::ifstream file("test1.nmea");

       if (file)
       {
           std::string line = getLastLine(file);
           std::cout << line << '\n';
           std::ofstream out("test2.nmea");
           out << line;
           out.close();
       }
       else
           std::cout << "Unable to open file.\n";

    /*Burayı ben ekledim Bitiş*/

       char line[80];
       static const char fileSource[] = "test2.nmea";
        static const char fileOutput[] = "test.json";

       FILE *in = fopen(fileSource, "r");
       FILE *out = fopen(fileOutput, "w");
       /* Check if filesource exists */
        if ( in != NULL )
         {
            /*Print header*/
            fprintf(out, "%s\n","{\"status\": \"OK\"," );
            /*Read file line by line*/
           while(fscanf (in, "%79[^\n]\n", line) == 1 ){
            // if GPRMC found filter it
              if(strstr(line, "$GPGGA")){
                // Get tokens from strings
                tokenize(line, out);
                //fprintf(out, "%s\n","," );
              }
           }
            /* Print footer*/
            fprintf(out, "%s\n","}}" );
            /* Close file*/
           fclose(out);
          }
        else
         {
            perror ( fileSource ); /* why didn't the file open? */
         }
        qDebug() << line;
}



/*tokenize() reads each line and takes tokens when divider found (comma)*/
void tokenize( char *oneline, FILE *fileName){

   const char delimiter[2] = DELIMITER__;
   char *token;
   int counter = 0;
   /* get the first token */
   token = strtok(oneline, delimiter);
   float ltemp;
   float accuracy;
   float latitude;
   float longitude;
   /* walk through other tokens */
   while( token != NULL ) {


       /*If 6th token == East multiply by -1*/
       if (counter == 8){/*convert the 4th token latitude*/
         ltemp = atof(token);
          accuracy = 0.02;
         //fprintf(fileName, "%s", "\"accuracy\": " );
         //fprintf(fileName,"%f,", ltemp );
       }

      /*convert the 4th token latitude*/
     if(counter == 2){
         ltemp = atof(token);
         ltemp = convertFromNmeaSentenceToDecimalCoord(ltemp,"m"); //"m" for meridian
     }
      /*If 5th token == South multiply by -1*/
      if (counter == 3){
        if (*token == 'S'){
          ltemp *= ( -1 );
        }
        latitude=ltemp;
        //fprintf(fileName,"%s", "\"lat\": ");
        //fprintf(fileName,"%f, ", ltemp );
      }
      /*convert the 6th token longitude*/
      if(counter == 4){
          ltemp = atof(token);
          ltemp = convertFromNmeaSentenceToDecimalCoord(ltemp, "p"); //"p" for parallel
      }
      /*If 6th token == East multiply by -1*/
      if (counter == 5){/*convert the 4th token latitude*/
        if (*token == 'W'){
          ltemp *= ( -1 );
        }
        longitude=ltemp;
        //fprintf(fileName, "%s", "\"long\": " );
        //fprintf(fileName,"%f,", ltemp );
      }
     /* if (counter == 7 ){
        ltemp = atof(token);
        ltemp = convertFromKnotsToKmHour(ltemp);
        fprintf(fileName, "info: 'Speed = %0.1f'}", ltemp);
      }*/
      token = strtok(NULL, delimiter);
      ++counter;
   }
   fprintf(fileName, "%s", "\"accuracy\": " );
   fprintf(fileName,"%f,", accuracy );
   fprintf(fileName, "%s", "\n" );
   fprintf(fileName, "%s", "\"location\": {\n" );
   fprintf(fileName, "%s", "\t\"lat\": " );
   fprintf(fileName,"%f,", latitude );
   fprintf(fileName, "%s", "\n" );
   fprintf(fileName, "%s", "\t\"lng\": " );
   fprintf(fileName,"%f", longitude );
   fprintf(fileName, "%s", "\n" );
}
/*selfexplantory*/
float convertFromNmeaSentenceToDecimalCoord(float coordinates, const char *val){
   /* Sample from gps 5153.6605*/
    /* Check limits*/
    if ((*val == 'm') && (coordinates < 0.0 && coordinates > MAX_LATITUDE)){
        return 0;
    }
    if (*val == 'p' && (coordinates < 0.0 && coordinates > MAX_LONGITUDE)){
          return 0;
    }
   int b;//to store the degrees
   float c; //to store de decimal

   /*Calculate the value in format nn.nnnnnn*/
   /*Explanations at:
      http://www.mapwindow.org/phorum/read.php?3,16271,16310*/

   b = coordinates/100; // 51 degrees
   c= (coordinates/100 - b)*100 ; //(51.536605 - 51)* 100 = 53.6605
   c /= 60; // 53.6605 / 60 = 0.8943417
   c += b; // 0.8943417 + 51 = 51.8943417
   return c;
}
/* Selfexplanatory*/
float convertFromKnotsToKmHour(float knots){
  knots = knots * KNOTS_TO_KM;
  return knots;
}
