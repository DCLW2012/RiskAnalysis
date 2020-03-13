
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFile>


#include "FileReader.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

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

	//!! ��־ϵͳ���洢��logĿ¼��

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
			return 0;
		}
	}
	
	return a.exec();
}
