#include <iostream>
#include <dirent.h>
#include <unistd.h>//current dir
#include <cstring>

#include <taglib/tag.h>
#include <taglib/taglib.h>
#include <taglib/fileref.h>

using namespace std;
using namespace TagLib;

int dir_elements(DIR**);
void file_array(DIR**,unsigned int,char[][256]);
void chmp3_attributes(unsigned int*,char[][256],char**,char**);
void shmp3_attributes(unsigned int*,char[][256]);

int main(int argc, char** argv)
{
	
	DIR *dir;
	unsigned int num = 0;
	int len = 0;
	
	if (string(argv[1])=="show")
	{
		chdir(argv[2]);
		dir = opendir(get_current_dir_name());
	}else{
		chdir(argv[3]);
		dir = opendir(get_current_dir_name());
	}
	
	
	num = dir_elements(&dir);
	char file[num][256]; //array nome dei file
	char (*p_file)[256] = file; //puntatore ai nomi dei file
	file_array(&dir,num,p_file);
	
	if (string(argv[1])=="show")
	{
		shmp3_attributes(&num,p_file);
	}else{
		chmp3_attributes(&num,p_file,&argv[1],&argv[2]);
	}
	
	closedir(dir);
	return 0;
}

int dir_elements(DIR** dir)
{
	unsigned int i = 0;
	struct dirent *dirent;
	while ((dirent = readdir(*dir)) != NULL)
		i += 1;
	rewinddir(*dir);
	return i-2;
}

void file_array(DIR** dir,unsigned int num,char v[][256])
{
	struct dirent *dirent;
	for (int i=0;i<num+2;i++)
	{
		dirent = readdir(*dir);
		if (i>1)
		{
			strncpy(v[i-2], dirent->d_name, sizeof(v[i-2]));
		}
	}
}

void chmp3_attributes(unsigned int* num,char file[][256],char** artist,char** album)
{
	int len;
	char *type;
	for (int i=0;i<*num;i++)
	{
		len = strlen(file[i]);
		type = &file[i][len-4];
		if (string(type)==".mp3")
		{
			TagLib::FileRef f(file[i]);
			f.tag()->setArtist(*artist);
			f.tag()->setAlbum(*album);
			cout << f.tag()->artist() << "  "
				<< f.tag()->title() << "  "
				<< f.tag()->album() << "\n";
			f.save();
		}
		
	}
}

void shmp3_attributes(unsigned int* num,char file[][256])
{
	int len;
	char *type;
	for (int i=0;i<*num;i++)
	{
		len = strlen(file[i]);
		type = &file[i][len-4];
		if (string(type)==".mp3")
		{
			TagLib::FileRef f(file[i]);
			cout << f.tag()->artist() << "  "
				<< f.tag()->title() << "  "
				<< f.tag()->album() << "\n";
		}
		
	}
}
