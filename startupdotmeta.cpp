#include <iostream>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <cstring>
#include <stdlib.h>

class settings
{
	char game_dir[501];
	char alt_dir[101];
public:
	settings()
	{
		strcpy(game_dir, "");
		strcpy(alt_dir, "startup.meta");
	}
	bool swap();
	void input_game_dir()
	{
		std::cout << "Please provide the directory/location of your Red Dead Redemption 2/Red Dead Online Game Folder:\nNOTE: Check C:\\Program Files\\Rockstar Games if you installed your game with the Rockstar Launcher and C:\\Program Files (x86)\\Steam\\steamapps\\common if Steam\n";
		std::cout << "Example Answers:\nC:\\Program Files\\Rockstar Games\\Red Dead Redemption 2\nC:\\Program Files (x86)\\Steam\\steamapps\\common\\Red Dead Redemption 2\n";
		std::string dir;
		std::getline(std::cin, dir);
		for(int i = 0; i < dir.length(); ++i)
			if(dir[i] == '\\')
				dir[i] = '/';
		dir += "/x64/data/startup.meta";
		strcpy(game_dir, dir.c_str());
	}
	void print_settings()
	{
		std::cout << "game_dir: " << game_dir << "\n" << "alt_dir: " << alt_dir << "\n";
	}
	bool dir_exists()
	{
		if(std::filesystem::exists(alt_dir) || std::filesystem::exists(game_dir))
			return true;
		else
			return false;
	}
};

bool settings::swap()
{
	char choice;
	if(std::filesystem::exists(game_dir) && std::filesystem::exists(alt_dir))
	{
		std::cerr << "ERROR: startup.meta exists in both locations\n";
		system("pause");
		return false;
	}
	else if(std::filesystem::exists(game_dir))
	{
		std::cout << "File is in\n" << game_dir << "\nDo you want to move it to\n" << alt_dir << "?\n(yes/no)\n";
		std::cin >> choice;
		if(choice == 'y')
		{
			try
			{
				std::filesystem::rename(game_dir, alt_dir);
			}
			catch(std::filesystem::filesystem_error& e)
			{
				std::cerr << e.what() << "\n";
				system("pause");
				return false;
			}
		}
	}
	else if(std::filesystem::exists(alt_dir))
	{
		std::cout << "File is in\n" << alt_dir << "\nDo you want to move it to\n" << game_dir << "?\n(yes/no)\n";
		std::cin >> choice;
		if(choice == 'y')
		{
			try
			{
				std::filesystem::rename(alt_dir, game_dir);
			}
			catch(std::filesystem::filesystem_error& e)
			{
				std::cerr << e.what() << "\n";
				system("pause");
				return false;
			}
		}
	}
	else
	{
		std::cerr << "ERROR: startup.meta doesn't exist in either location\n";
		system("pause");
		return false;
	}
	return true;
}

int main()
{
	char download_url[] = "https://drive.google.com/uc?id=1ASmeWi0opAzKjorXdNa_CTkpC7bPqj7Q&export=download";
	char local_file[] = "startup.meta";
	std::fstream setfile;
	settings obj;
	if(std::filesystem::exists("settings.dat"))
	{
		std::cout << "Settings file detected, proceeding with prior information\n";
		setfile.open("settings.dat", std::ios::in | std::ios::binary);
		setfile.read((char*)&obj, sizeof(settings));
	}
	else
	{
		std::cout << "Settings file not detected, proceeding with first time setup\n";
		obj.input_game_dir();
		setfile.open("settings.dat", std::ios::out | std::ios::binary);
		setfile.write((char*)&obj, sizeof(settings));
	}
	setfile.close();
	obj.print_settings();
	if(!obj.dir_exists())
	{
		std::cout << "WARNING: startup.meta file does not exist in either location, starting download to local folder where you executed this program from.\n";
		if(URLDownloadToFile(NULL, download_url, local_file, 0, NULL) == S_OK)
			std::cout << "Download successful\n";
		else
		{
			std::cerr << "Download failed, aborting program\n";
			system("pause");
			return(-1);
		}
	}
	if(obj.swap())
	{
		std::cout << "Clean exit\n";
		return 0;
	}
	else
	{
		std::cerr << "Aborting program\n";
		system("pause");
		return(-1);
	}
}