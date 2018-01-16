#include <stdio.h>
#include <QLatin1String>
#include "recognitiononline.h"

RecognitionOnline::RecognitionOnline()
{
    grammar_id_len = 128;
    login_params = "appid = 587de813, work_dir = .";                           //登陆参数，appid表示在线注册的应用ID
    grammar_id = NULL;
    result = NULL;
    isLogin = false;
}

bool RecognitionOnline::doLogin(const char* username, const char* password)
{
	int info_code = MSPLogin(username, password, login_params);
	if (info_code != MSP_SUCCESS) {
		errorInfo.push(QString(QLatin1String("Error : Login failed . ")) + QString::number(info_code, 10));
		return false;
	}
	isLogin = true;
	return true;
}

bool RecognitionOnline::uploadData(const char* location, const char* mode)
{
	FILE* fp = NULL;
	int info_code = -1;
	char* grammar = NULL;
	int grammar_len = -1;

	if ((fp = fopen(location, "rb")) == NULL) {
		errorInfo.push(QString(QLatin1String("Error : Upload data file open failed . ")));
		return false;
	}

	fseek(fp, 0, SEEK_END);
	grammar_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	grammar = new char[grammar_len + 1];
	if (grammar == NULL) {
		errorInfo.push(QString(QLatin1String("Error : Allocate upload data file memory failed . ")));
		return false;
	}
	else {
		fread(grammar, 1, grammar_len, fp);
		grammar[grammar_len] = 0;
	}

	char* tmp_grammar_id = new char[100];
	strcpy(tmp_grammar_id, MSPUploadData("usergram", grammar, grammar_len, mode, &info_code));

	if (info_code != MSP_SUCCESS) {
		errorInfo.push(QString(QLatin1String("Error : Upload data file failed . ")) + QString::number(info_code, 10));
		delete grammar;
		return false;
	}

	grammar_id = new char[strlen(tmp_grammar_id) + 1];
	strcpy(grammar_id, tmp_grammar_id);

	fclose(fp);
	delete grammar;
	delete tmp_grammar_id;
	return true;
}

char* RecognitionOnline::recognition(const char* location, const char* mode)
{
	FILE* fp = NULL;
	char* voice = NULL;
	int voice_len = -1;
	int info_code = -1;
	char* session_id = NULL;
	int result_len = 0;

	int audio_status = MSP_AUDIO_SAMPLE_CONTINUE;                              //音频进行状态
	int endpo_status = MSP_EP_LOOKING_FOR_SPEECH;                              //断点检测状态-from server
	int recog_status = MSP_REC_STATUS_SUCCESS;                                 //语音识别状态

	if ((fp = fopen(location, "rb")) == NULL) {
		errorInfo.push(QString(QLatin1String("Error : Wav voice file open failed . ")));
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	voice_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	voice = new char[voice_len];
	if (voice == NULL) {
		errorInfo.push(QString(QLatin1String("Error : Allocate voice file memory failed . ")));
		return NULL;
	}
	fread(voice, 1, voice_len, fp);

	char* tmp_session_id = new char[100];
	strcpy(tmp_session_id, QISRSessionBegin(grammar_id, mode, &info_code));
	if (info_code != MSP_SUCCESS) {
		errorInfo.push(QString(QLatin1String("Error : Recognition online begin failed . ")) + QString::number(info_code, 10));
		delete voice;
		delete tmp_session_id;
		return NULL;
	}
	session_id = new char[strlen(tmp_session_id) + 1];
	strcpy(session_id, tmp_session_id);
	delete tmp_session_id;

	int tmp_space = 0;
	while (true) {
		int upload_len = RATE * SIZE / 5 / 8;
		if (voice_len < 2 * upload_len) {                                        //如果剩下的上传需要一次多一点，那么就一次上传结束
			upload_len = voice_len;
		}

		audio_status = tmp_space == 0 ? MSP_AUDIO_SAMPLE_FIRST : MSP_AUDIO_SAMPLE_CONTINUE;

		info_code = QISRAudioWrite(session_id, (const void*)&voice[tmp_space], upload_len, audio_status, &endpo_status, &recog_status);
		if (info_code != MSP_SUCCESS) {
			errorInfo.push(QString(QLatin1String("Error : Recognition online write audio failed . ")) + QString::number(info_code, 10));
			delete voice;
			return NULL;
		}

		tmp_space += upload_len;
		voice_len -= upload_len;

		if (endpo_status == MSP_EP_AFTER_SPEECH || voice_len == 0) {
			break;
		}

		//usleep(200*1000);                                                      //本人觉得不需要这句
	}
	info_code = QISRAudioWrite(session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &endpo_status, &recog_status);    //告知服务器上传结束
	delete voice;
	if (info_code != MSP_SUCCESS) {
		errorInfo.push(QString(QLatin1String("Error : Recognition online write audio (end) failed . ")) + QString::number(info_code, 10));
		return NULL;
	}

	result = new char[200];
	while (recog_status != MSP_REC_STATUS_COMPLETE) {
		const char* tmp_result = QISRGetResult(session_id, &recog_status, 0, &info_code);
		if (info_code != MSP_SUCCESS) {
			errorInfo.push(QString(QLatin1String("Error : Recognition online get result failed . ")) + QString::number(info_code, 10));
			return NULL;
		}
		if (tmp_result != NULL) {
			unsigned int tmp_result_len = strlen(tmp_result);
			result_len += tmp_result_len;
			if (result_len > 200) {
				break;
			}
			strncat(result, tmp_result, tmp_result_len);
		}
		//usleep(150 * 1000);                                                    //防止频繁占用CPU    本人觉得不需要
	}

	QISRSessionEnd(session_id, "info : End recognition online . ");
	delete session_id;
	return result;
}

char* RecognitionOnline::getGrammarId()
{
	return grammar_id;
}

char* RecognitionOnline::getTopResult()
{
	char* first = strstr(result, "input=");
	first += 6;
	char* second = strstr(first, "confidence");

	char* tmp_result = new char[100];
	strncpy(tmp_result, first, (second - first));
	return tmp_result;
}

char* RecognitionOnline::getResult()
{
	return result;
}
