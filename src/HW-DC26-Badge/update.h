#ifndef HW_UPDATE_H
#define HW_UPDATE_H

void performUpdate(Stream &updateSource, size_t updateSize) {
   if (Update.begin(updateSize)) {      
      size_t written = Update.writeStream(updateSource);
      if (written == updateSize) {
         //Serial.println("Written : " + String(written) + " successfully");
      }
      else {
         //Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
      }
      if (Update.end()) {
         //Serial.println("OTA done!");
         if (Update.isFinished()) {
            ESP.restart();
            //Serial.println("Update successfully completed. Rebooting.");
         }
         else {
            //Serial.println("Update not finished? Something went wrong!");
         }
      }
      else {
         //Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
   }
   else
   {
      //Serial.println("Not enough space to begin OTA");
   }
}

void updateFromFS(fs::FS &fs) {
   File updateBin = fs.open("/update.bin");
   if (updateBin) {
      if(updateBin.isDirectory()){
         //Serial.println("Error, update.bin is not a file");
         updateBin.close();
         return;
      }
      size_t updateSize = updateBin.size();
      if (updateSize > 0) {
         //Serial.println("Try to start update");
         performUpdate(updateBin, updateSize);
      }
      else {
         //Serial.println("Error, file is empty");
      }
      updateBin.close();
      //fs.remove("/update.bin");      // we finished remove the binary from sd card to indicate end of the process
   }
   else {
      //Serial.println("Could not load update.bin from sd root");
   }
}

#endif
