
#include <QtCore/QCoreApplication>
#include "FileReader.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//! ��ǰ����·��

	//!! ��־ϵͳ

	//! ��ȡ�ļ�
	FileReader fReader = new FileReader(nullptr);

	//! �������

	return a.exec();
}
