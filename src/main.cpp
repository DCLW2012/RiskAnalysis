
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFile>
#include "QTextCodec"

#include "FileReader.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//���ʻ�
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

	//!! ��־ϵͳ���洢��logĿ¼��
	//! ��ʼ������log�ļ�
	Logger::CreateNewLogFile(QCoreApplication::applicationName());

	//! ��ǰ����·��
	QString curDirectory = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());

	QString databaseFullPath = curDirectory + "\\data\\database.txt";
	QString dbParamsFullPath = curDirectory + "\\data\\database_params.txt";
	QString inputFullPath = curDirectory + "\\data\\input.txt";
	QString outputFullPath = curDirectory + "\\data\\output.txt";
	
	//! ÿ�μ�����ɾ��outputFullPath��Ӧ���ļ�
	QFile file(outputFullPath);
	if (file.exists())
	{
		file.remove();
    }

	//! ��ȡ�ļ�
	FileReader fReader = new FileReader(nullptr);
	bool isdbSuccess = fReader.ReadDatabase(databaseFullPath);
	bool isdbparamSuccess = fReader.ReadDatabaseParam(dbParamsFullPath);

	//! �������
	if(isdbSuccess && isdbparamSuccess)
	{
		bool isSuccess = fReader.ReadInputForOut(inputFullPath, outputFullPath);

		if(isSuccess)
		{
			a.quit();
			Logger::Message(QString("����ɹ���"));
			return 0;
		}
		Logger::Message(QString("�����ļ�������߼���ʧ�ܣ�"));
	}else{
		Logger::Message(QString("���ݿ��ļ����߲����ļ���������ȷ��"));
	}
	
	return a.exec();
}
