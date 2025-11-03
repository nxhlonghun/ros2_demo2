#ifndef COMMON_CLASS
#define COMMON_CLASS

#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QLayout>
#include <QLabel>
#include <QDateTime>

#define MAX_NODE_NUM 256

#define hdebug(format, ...) qDebug("[%s:%d] [%s] " format, __FUNCTION__, __LINE__, getDescription().toStdString().c_str(), __VA_ARGS__)
#define hinfo(format, ...) qInfo("[%s:%d] [%s] " format, __FUNCTION__, __LINE__, getDescription().toStdString().c_str(), __VA_ARGS__)
#define hwarn(format, ...) qWarning("[%s:%d] [%s] " format, __FUNCTION__, __LINE__, getDescription().toStdString().c_str(), __VA_ARGS__)
#define herror(format, ...) qWarning("[error] [%s:%d] [%s] " format, __FUNCTION__, __LINE__, getDescription().toStdString().c_str(), __VA_ARGS__)
#define hdebug2(format, ...) qDebug("hdebug2 [%s:%d] [%s] " format, __FUNCTION__, __LINE__, getDescription().toStdString().c_str(), __VA_ARGS__)

#define Q2CSTR(str) str.toStdString().c_str()
#define S2QSTR(str) QString::fromStdString(str)
#define J2CSTR(json) QJsonDocument(json).toJson(QJsonDocument::Compact).toStdString().c_str()
#define J2QSTR(json) QJsonDocument(json).toJson(QJsonDocument::Compact)

#define CMPT_NAME getDescription()

#define DEVICE_OFFLINE_WORK_MODE 0		// 单机模式
#define DEVICE_ONLINE_WORK_MODE 1		// 联机模式
#define DEVICE_ONLINE_NOT_WORK_MODE 2	// 联机直通模式
#define DEVICE_DEBUG_WORK_MODE 3		// 调试模式
#define DEVICE_PRESURE_TEST_WORK_MODE 4 // 压力测试模式

class CommonClass : public QObject
{
	Q_OBJECT
public:
private:
public:
	CommonClass();

	~CommonClass();

	static CommonClass *getInstance();

	static bool isJsonString(QString value);

	static QJsonObject getJsonObjectFromString(QString value);

	static QJsonArray getJsonArrayFromString(QString value);

	static QString getStringFromJsonObject(QJsonObject value);

	static QString getStringFromJsonArray(QJsonArray value);

	static QJsonObject loadJsonFileObject(QString fileName, bool hasChinese = false);

	static QJsonArray loadJsonFileArray(QString fileName);

	static QJsonObject loadJsonFileObject();

	static QJsonArray loadJsonFileArray();

	static QString QByteArray_TO_QString(QByteArray BYTE);

	static QByteArray QString_TO_QByteArray(QString STR);

	static QString loadFile(QString fileName);

	static void writeFile(QByteArray data, QString fileName);

	static QString loadJsonFileString(QString fileName);

	static bool saveJsonObj2OpenFile(QJsonObject obj);

	static bool saveJsonArray2OpenFile(QJsonArray arr);

	static bool loadJsonObjFromOpenFile(QJsonObject &result);

	static bool loadJsonArrayFromOpenFile(QJsonArray &result);

	static void writeJsonFileArray(QJsonArray obj, QString filePath);

	static void writeJsonFileObj(QJsonObject obj, QString filePath);

	static void writeStringFileObj(QString strValue, QString filePath);
};

#endif