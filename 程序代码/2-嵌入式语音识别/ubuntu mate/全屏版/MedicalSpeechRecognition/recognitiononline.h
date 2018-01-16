#include <QStack>
#include "voice.h"
#include "xfMSC/qisr.h"
#include "xfMSC/msp_cmn.h"
#include "xfMSC/msp_errors.h"

class RecognitionOnline
{
private:
    int grammar_id_len;
    char* login_params;
    char* grammar_id;
    char* result;
public:
    bool isLogin;
	QStack<QString> errorInfo;
    RecognitionOnline();
	bool doLogin(const char* username, const char* password);
	bool uploadData(const char* location, const char* mode);
	char* recognition(const char* location, const char* mode);

	char* getGrammarId();
	char* getTopResult();
	char* getResult();
};
