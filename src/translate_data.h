#pragma once

class Model;
class HumansMigration;

/*
	������� ������������ ������� ������ �� ���������� �������� � ���������� ���������
*/
namespace translate
{
	int translate_data(Model* model, HumansMigration *phm);//����������� ���������������� ������
	int translate_step (Model* model, HumansMigration* phm, int day_idx); // ���������� ������ � ���������� ����������
};
