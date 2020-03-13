#include "DataType.h"

Database::Database()
	: QObject()
{

}

Database::~Database()
{

}

int Database::GetFieldNum()
{
	return m_fields.size();
}

FieldMap& Database::GetFields()
{
	return m_fields;
}


//*****************************************************************
DatabaseParams::DatabaseParams()
	: QObject()
{

}

DatabaseParams::~DatabaseParams()
{

}
