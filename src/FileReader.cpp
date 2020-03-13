#include "FileReader.h"

FileReader::FileReader(QObject *parent)
	: QObject(parent)
{

}

FileReader::~FileReader()
{

}

bool FileReader::ReadDatabase(QString fileName)
{

	return true;
}


bool FileReader::ReadDatabaseParam(QString fileName)
{
	return true;
}

bool FileReader::ReadInput(QString fileName)
{
	return true;
}