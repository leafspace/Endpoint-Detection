#include <QStack>
#include "xfMSC/qisr.h"
#include "voice.h"
#include "xfMSC/msp_cmn.h"
#include "xfMSC/msp_errors.h"

class RecognitionOnline
{
private:
    const int grammar_id_len = 128;
    const char* login_params = "appid = 587de813, work_dir = .";               //登陆参数，appid表示在线注册的应用ID
    char* grammar_id = NULL;

public:
    QStack<QString> errorInfo;
    bool doLogin(const char* username, const char* password);
    bool uploadData(const char* location, const char* mode);
    char* recognition(const char* location, const char* mode);
};
