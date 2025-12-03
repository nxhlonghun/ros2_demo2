#include "demo_cpp_qt0/CommonClass.h"
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStorageInfo>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QJsonParseError>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QDirIterator>
#include <QSettings>

#include <QDeadlineTimer>
#include <QThread>

CommonClass::CommonClass()
{
}

CommonClass::~CommonClass()
{
}

CommonClass *CommonClass::getInstance()
{
	static CommonClass instance;
	;
	return &instance;
}

bool CommonClass::isJsonString(QString value)
{
	// 判断valueText是否为jsonObject，如果是则显示部分
	bool isJson = false;
	QJsonParseError jsonErr;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(value.toUtf8(), &jsonErr);

	if (!jsonDocument.isNull() && (jsonErr.error == QJsonParseError::NoError))
	{
		if (jsonDocument.isObject())
		{
			return true;
		}
		else if (jsonDocument.isArray())
		{
			return true;
		}
	}

	return false;
}

QJsonObject CommonClass::getJsonObjectFromString(QString value)
{
	QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8().data());
	return doc.object();
}

QJsonArray CommonClass::getJsonArrayFromString(QString value)
{
	QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8().data());
	return doc.array();
}

QString CommonClass::getStringFromJsonObject(QJsonObject value)
{
	return QString(QJsonDocument(value).toJson(QJsonDocument::Compact));
}

QString CommonClass::getStringFromJsonArray(QJsonArray value)
{
	return QString(QJsonDocument(value).toJson(QJsonDocument::Compact));
}

QJsonObject CommonClass::loadJsonFileObject(QString fileName, bool hasChinese)
{
	QJsonObject fileObj;

	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite))
	{
		return fileObj;
	}

	QByteArray wholeFile = file.readAll();

	if (hasChinese)
		wholeFile = QString::fromLocal8Bit(wholeFile).toUtf8();

	QJsonParseError jsonErr;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(wholeFile, &jsonErr);
	if (!jsonDocument.isNull() && (jsonErr.error == QJsonParseError::NoError))
	{
		if (jsonDocument.isObject())
		{
			fileObj = jsonDocument.object();
		}
	}

	return fileObj;
}

QJsonArray CommonClass::loadJsonFileArray(QString fileName)
{
	QJsonArray fileArr;

	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite))
	{
		return fileArr;
	}

	QByteArray wholeFile = file.readAll();
	QJsonParseError jsonErr;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(wholeFile, &jsonErr);
	if (!jsonDocument.isNull() && (jsonErr.error == QJsonParseError::NoError))
	{
		if (jsonDocument.isArray())
		{
			fileArr = jsonDocument.array();
		}
	}

	return fileArr;
}

QJsonObject CommonClass::loadJsonFileObject()
{
	QJsonObject fileObj;

	QString str1 = "打开文件";
	QString str2 = "参数文件 (*.json);";

	QString fileName = QFileDialog::getOpenFileName(nullptr, str1, ".", str2);
	if (!QFileInfo::exists(fileName))
	{
		return fileObj;
	}

	return loadJsonFileObject(fileName);
}

QJsonArray CommonClass::loadJsonFileArray()
{
	QJsonArray fileArr;

	QString str1 = "打开文件";
	QString str2 = "参数文件 (*.json);";

	QString fileName = QFileDialog::getOpenFileName(nullptr, str1, ".", str2);
	if (!QFileInfo::exists(fileName))
	{
		return fileArr;
	}

	return loadJsonFileArray(fileName);
}

QString CommonClass::QByteArray_TO_QString(QByteArray BYTE)
{
	return QString::fromLatin1(BYTE.toHex());
}

QByteArray CommonClass::QString_TO_QByteArray(QString STR)
{
	return QByteArray::fromHex(STR.toLatin1());
}

void CommonClass::writeFile(QByteArray data, QString fileName)
{
	QFile file(fileName);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(data);
		file.close();
	}
}

QString CommonClass::loadFile(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return "";
	}

	QByteArray wholeFile = file.readAll();

	QString str = QByteArray_TO_QString(wholeFile);

	file.close();

	return str;
}

QString CommonClass::loadJsonFileString(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite))
	{
		return "";
	}

	QByteArray wholeFile = file.readAll();
	QJsonParseError jsonErr;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(wholeFile, &jsonErr);
	if (!jsonDocument.isNull() && (jsonErr.error == QJsonParseError::NoError))
	{
		return QString(wholeFile);
	}
	else
		return "";
}

bool CommonClass::saveJsonObj2OpenFile(QJsonObject obj)
{
	QString str1 = "保存文件";
	QString str2 = "数据文件 (*.json);";

	QSettings setting(QCoreApplication::applicationDirPath() + "/CommonClass.ini", QSettings::IniFormat);
	QString fileName = QFileDialog::getSaveFileName(nullptr, str1, setting.value("last_save_as_path", ".").toString(),
													str2);
	if (!fileName.isNull())
	{
		setting.setValue("last_save_as_path", fileName);

		writeJsonFileObj(obj, fileName);

		return true;
	}
	else
		return false;
}

bool CommonClass::saveJsonArray2OpenFile(QJsonArray arr)
{
	QString str1 = "保存文件";
	QString str2 = "数据文件 (*.json);";

	QSettings setting(QCoreApplication::applicationDirPath() + "/CommonClass.ini", QSettings::IniFormat);
	QString fileName = QFileDialog::getSaveFileName(nullptr, str1, setting.value("last_save_as_path", ".").toString(),
													str2);
	if (!fileName.isNull())
	{
		setting.setValue("last_save_as_path", fileName);

		writeJsonFileArray(arr, fileName);

		return true;
	}
	else
		return false;
}

bool CommonClass::loadJsonObjFromOpenFile(QJsonObject &result)
{
	QString str1 = "打开文件";
	QString str2 = "数据文件 (*.json);";

	QSettings setting(QCoreApplication::applicationDirPath() + "/CommonClass.ini", QSettings::IniFormat);
	QString fileName = QFileDialog::getOpenFileName(nullptr, str1, setting.value("last_open_path", ".").toString(),
													str2);
	if (QFileInfo::exists(fileName))
	{
		setting.setValue("last_open_path", fileName);
		result = CommonClass::loadJsonFileObject(fileName);
		return true;
	}
	else
		return false;
}

bool CommonClass::loadJsonArrayFromOpenFile(QJsonArray &result)
{
	QString str1 = "打开文件";
	QString str2 = "数据文件 (*.json);";

	QSettings setting(QCoreApplication::applicationDirPath() + "/CommonClass.ini", QSettings::IniFormat);
	QString fileName = QFileDialog::getOpenFileName(nullptr, str1, setting.value("last_open_path", ".").toString(),
													str2);
	if (QFileInfo::exists(fileName))
	{
		setting.setValue("last_open_path", fileName);
		result = loadJsonFileArray(fileName);
		return true;
	}
	else
		return false;
}

void CommonClass::writeJsonFileArray(QJsonArray obj, QString filePath)
{
	QJsonDocument document(obj);

	QFile file(filePath);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(document.toJson());
		file.close();
	}
}
void CommonClass::writeJsonFileObj(QJsonObject obj, QString filePath)
{
	QJsonDocument document(obj);

	QFile file(filePath);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(document.toJson());
		file.close();
	}
}

void CommonClass::writeStringFileObj(QString strValue, QString filePath)
{
	QFile file(filePath);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(strValue.toUtf8());
		file.close();
	}
}