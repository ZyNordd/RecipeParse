#include"eda.h"

#ifdef _WIN32
extern "C" __declspec(dllexport) std::string ingredients(std::string htmlcode) {
	std::string str(htmlcode);

	std::string triggerPorc("class=");
	triggerPorc.push_back('"');
	triggerPorc.insert(7, "emotion-175lmk7", 0, 15);
	triggerPorc.push_back('"');
	triggerPorc.push_back('>');

	std::string triggerCount("class=");
	triggerCount.push_back('"');
	triggerCount.insert(7, "emotion-1047m5l", 0, 15);
	triggerCount.push_back('"');
	triggerCount.push_back('>');

	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "recipeIngredient", 0, 16);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string ingr;

	auto index = str.find(triggerCount);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	while (str[index + 24 + i] != '<') {
		ingr.push_back(str[index + 24 + i]);
		++i;
	}
	ingr.push_back(' ');

	index = str.find(triggerPorc);
	i = 0;
	while (str[index + 24 + i] != '<') {
		ingr.push_back(str[index + 24 + i]);
		++i;
	}
	ingr.push_back(':');
	ingr.push_back('\n');


	index = str.find(trigger);
	i = 0;
	bool flag = 1;
	while (str[index + 28 + i] != '<') {
		ingr.push_back(str[index + 28 + i]);
		++i;
	}
	ingr.push_back(' ');
	ingr.push_back('-');
	ingr.push_back(' ');
	while (str[index + 124 + i] != '<') {
		ingr.push_back(str[index + 124 + i]);
		++i;
	}
	while (flag) {
		index = str.find(trigger, index + 1);
		if (index == std::string::npos) {
			flag = 0;
			continue;
		}
		i = 0;
		ingr.push_back('\n');
		while (str[index + 28 + i] != '<') {
			ingr.push_back(str[index + 28 + i]);
			++i;
		}
		ingr.push_back(' ');
		ingr.push_back('-');
		ingr.push_back(' ');
		while (str[index + 124 + i] != '<') {
			ingr.push_back(str[index + 124 + i]);
			++i;
		}
	}
	return ingr;
}

extern "C" __declspec(dllexport) std::string name(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("meta itemProp=");
	trigger.push_back('"');
	trigger.insert(15, "keywords", 0, 8);
	trigger.push_back('"');
	trigger.insert(24, " content=", 0, 9);
	trigger.push_back('"');
	std::string str_name;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 34 + i] != '"') {
		str_name.push_back(str[index + 34 + i]);
		++i;
	}
	return str_name;
}

extern "C" __declspec(dllexport) int calories(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "calories", 0, 8);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_cal;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 20 + i] != '<') {
		str_cal.push_back(str[index + 20 + i]);
		++i;
	}	
	int num_cal = stoi(str_cal);
	return num_cal;
}

extern "C" __declspec(dllexport) int protein(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "proteinContent", 0, 14);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_prot;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 26 + i] != '<') {
		str_prot.push_back(str[index + 26 + i]);
		++i;
	}
	int num_prot = stoi(str_prot);
	return num_prot;
}

extern "C" __declspec(dllexport) int fat(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "fatContent", 0, 10);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_fat;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 22 + i] != '<') {
		str_fat.push_back(str[index + 22 + i]);
		++i;
	}
	int num_fat = stoi(str_fat);
	return num_fat;
}

extern "C" __declspec(dllexport) int carbohydrate(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "carbohydrateContent", 0, 19);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_carb;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 31 + i] != '<') {
		str_carb.push_back(str[index + 31 + i]);
		++i;
	}
	int num_carb = stoi(str_carb);
	return num_carb;
}

extern "C" __declspec(dllexport) std::string recipe(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "text", 0, 4);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_text("1. ");
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 16 + i] != '<') {
		str_text.push_back(str[index + 16 + i]);
		++i;
	}
	int n = 2;
	while (flag) {
		index = str.find(trigger, index + 1);
		if (index == std::string::npos) {
			flag = 0;
			continue;
		}
		i = 0;
		str_text.push_back('\n');
		std::string n_str = std::to_string(n);
		str_text.insert(str_text.size(),n_str,0, n_str.size());
		str_text.push_back('.');
		str_text.push_back(' ');
		n++;
		while (str[index + 16 + i] != '<') {
			str_text.push_back(str[index + 16 + i]);
			++i;
		}
	}
	return str_text;
}

extern "C" __declspec(dllexport) std::string URLs(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("class=");
	trigger.push_back('"');
	trigger.insert(7, "emotion-1eugp2w", 0, 15);
	trigger.push_back('"');
	std::string URList;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	bool flag = 1;
	std::string domain = "https://eda.ru";
	URList.insert(URList.size(), domain, 0, 15);
	while (str[index + 33 + i] != '"') {
		URList.push_back(str[index + 33 + i]);
		++i;
	}
	int n = 2;
	while (flag) {
		index = str.find(trigger, index + 1);
		if (index == std::string::npos) {
			flag = 0;
			continue;
		}
		i = 0;
		URList.push_back('\n');
		URList.insert(URList.size(), domain, 0, 15);
		while (str[index + 33 + i] != '"') {
			URList.push_back(str[index + 33 + i]);
			++i;
		}
	}
	URList.push_back('\n');
	return URList;
}

#else
extern "C" std::string ingredients(std::string htmlcode) {
	std::string str(htmlcode);

	std::string triggerPorc("class=");
	triggerPorc.push_back('"');
	triggerPorc.insert(7, "emotion-175lmk7", 0, 15);
	triggerPorc.push_back('"');
	triggerPorc.push_back('>');

	std::string triggerCount("class=");
	triggerCount.push_back('"');
	triggerCount.insert(7, "emotion-1047m5l", 0, 15);
	triggerCount.push_back('"');
	triggerCount.push_back('>');

	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "recipeIngredient", 0, 16);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string ingr;

	auto index = str.find(triggerCount);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	while (str[index + 24 + i] != '<') {
		ingr.push_back(str[index + 24 + i]);
		++i;
	}
	ingr.push_back(' ');

	index = str.find(triggerPorc);
	i = 0;
	while (str[index + 24 + i] != '<') {
		ingr.push_back(str[index + 24 + i]);
		++i;
	}
	ingr.push_back(':');
	ingr.push_back('\n');


	index = str.find(trigger);
	i = 0;
	bool flag = 1;
	while (str[index + 28 + i] != '<') {
		ingr.push_back(str[index + 28 + i]);
		++i;
	}
	ingr.push_back(' ');
	ingr.push_back('-');
	ingr.push_back(' ');
	while (str[index + 124 + i] != '<') {
		ingr.push_back(str[index + 124 + i]);
		++i;
	}
	while (flag) {
		index = str.find(trigger, index + 1);
		if (index == std::string::npos) {
			flag = 0;
			continue;
		}
		i = 0;
		ingr.push_back('\n');
		while (str[index + 28 + i] != '<') {
			ingr.push_back(str[index + 28 + i]);
			++i;
		}
		ingr.push_back(' ');
		ingr.push_back('-');
		ingr.push_back(' ');
		while (str[index + 124 + i] != '<') {
			ingr.push_back(str[index + 124 + i]);
			++i;
		}
	}
	return ingr;
}

extern "C" std::string name(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("meta itemProp=");
	trigger.push_back('"');
	trigger.insert(15, "keywords", 0, 8);
	trigger.push_back('"');
	trigger.insert(24, " content=", 0, 9);
	trigger.push_back('"');
	std::string str_name;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 34 + i] != '"') {
		str_name.push_back(str[index + 34 + i]);
		++i;
	}
	return str_name;
}

extern "C" int calories(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "calories", 0, 8);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_cal;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 20 + i] != '<') {
		str_cal.push_back(str[index + 20 + i]);
		++i;
	}
	int num_cal = stoi(str_cal);
	return num_cal;
}

extern "C" int protein(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "proteinContent", 0, 14);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_prot;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 26 + i] != '<') {
		str_prot.push_back(str[index + 26 + i]);
		++i;
	}
	int num_prot = stoi(str_prot);
	return num_prot;
}

extern "C" int fat(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "fatContent", 0, 10);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_fat;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 22 + i] != '<') {
		str_fat.push_back(str[index + 22 + i]);
		++i;
	}
	int num_fat = stoi(str_fat);
	return num_fat;
}

extern "C" int carbohydrate(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "carbohydrateContent", 0, 19);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_carb;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return 0;
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 31 + i] != '<') {
		str_carb.push_back(str[index + 31 + i]);
		++i;
	}
	int num_carb = stoi(str_carb);
	return num_carb;
}

extern "C" std::string recipe(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("itemProp");
	trigger.push_back('=');
	trigger.push_back('"');
	trigger.insert(10, "text", 0, 4);
	trigger.push_back('"');
	trigger.push_back('>');
	std::string str_text("1. ");
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	bool flag = 1;
	while (str[index + 16 + i] != '<') {
		str_text.push_back(str[index + 16 + i]);
		++i;
	}
	int n = 2;
	while (flag) {
		index = str.find(trigger, index + 1);
		if (index == std::string::npos) {
			flag = 0;
			continue;
		}
		i = 0;
		str_text.push_back('\n');
		std::string n_str = std::to_string(n);
		str_text.insert(str_text.size(), n_str, 0, n_str.size());
		str_text.push_back('.');
		str_text.push_back(' ');
		n++;
		while (str[index + 16 + i] != '<') {
			str_text.push_back(str[index + 16 + i]);
			++i;
		}
	}
	return str_text;
}

extern "C" std::string URLs(std::string htmlcode) {
	std::string str(htmlcode);
	std::string trigger("class=");
	trigger.push_back('"');
	trigger.insert(7, "emotion-1eugp2w", 0, 15);
	trigger.push_back('"');
	std::string URList;
	auto index = str.find(trigger);
	if (index == std::string::npos) {
		return "";
	}
	int i = 0;
	bool flag = 1;
	std::string domain = "https://eda.ru";
	URList.insert(URList.size(), domain, 0, 15);
	while (str[index + 33 + i] != '"') {
		URList.push_back(str[index + 33 + i]);
		++i;
	}
	int n = 2;
	while (flag) {
		index = str.find(trigger, index + 1);
		if (index == std::string::npos) {
			flag = 0;
			continue;
		}
		i = 0;
		URList.push_back('\n');
		URList.insert(URList.size(), domain, 0, 15);
		while (str[index + 33 + i] != '"') {
			URList.push_back(str[index + 33 + i]);
			++i;
		}
	}
	URList.push_back('\n');
	return URList;
}

#endif