class Mp3Notify;
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;

class Mp3Notify {
  public:
    static void OnError(DfMp3& mp3, uint16_t errorCode);
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action);
    static void OnPlayFinished(DfMp3& mp3, DfMp3_PlaySources source, uint16_t track);
    static void OnPlaySourceOnline(DfMp3& mp3, DfMp3_PlaySources source);
    static void OnPlaySourceInserted(DfMp3& mp3, DfMp3_PlaySources source);
    static void OnPlaySourceRemoved(DfMp3& mp3, DfMp3_PlaySources source);
};

extern DfMp3 mp3;
void setupMp3Player();
