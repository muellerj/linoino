class Mp3Notify {
  public:
    static void OnError(uint16_t errorCode) {
      Serial.println("Com Error " + String(errorCode));
    }

    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
       if (source & DfMp3_PlaySources_Sd) Serial.print("SD Karte ");
       if (source & DfMp3_PlaySources_Usb) Serial.print("USB ");
       if (source & DfMp3_PlaySources_Flash) Serial.print("Flash ");
       Serial.println(action);
     }

    static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {
      Serial.println("End of track " + String(track));
      delay(100);
      nextTrack(track);
    }

    static void OnPlaySourceOnline(DfMp3_PlaySources source) {
      PrintlnSourceAction(source, "online");
    }

    static void OnPlaySourceInserted(DfMp3_PlaySources source) {
       PrintlnSourceAction(source, "ready");
     }

    static void OnPlaySourceRemoved(DfMp3_PlaySources source) {
      PrintlnSourceAction(source, "removed");
    }
};

DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);
