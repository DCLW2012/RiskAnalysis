#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>


class Database;
class FileReader : public QObject
{
	Q_OBJECT

public:
	FileReader(QObject *parent);
	~FileReader();

	//! 1����ȡ���ݿ��������ļ�
	bool ReadDatabase(QString fileName);

	//! 2����ȡ���Ʋ����ļ�
	bool ReadDatabaseParam(QString fileName);

	//! 3����ȡ�����ļ����������
	bool ReadInput(QString fileName);
private:
	Database* m_database;
};

#endif // FILEREADER_H
