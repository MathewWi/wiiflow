#include "cachedlist.hpp"

template <class T>
void CachedList<T>::Load(string path, string containing, u32 *count)													/* Load All */
{
	gprintf("\nLoading files containing %s in %s\n", containing.c_str(), path.c_str());
	m_loaded = false;
	m_database = sfmt("%s/%s.db", m_cacheDir.c_str(), (make_db_name(path)).c_str());
	
	gprintf("Database file: %s\n", m_database.c_str());
	m_wbfsFS = strncasecmp(DeviceHandler::Instance()->PathToFSName(path.c_str()), "WBFS", 4) == 0;
	
	if(force_update)
	{
		remove(m_database.c_str());
		force_update = false;
	}
	
	if(!m_wbfsFS)
	{
		struct stat filestat, cache;

		if(stat(path.c_str(), &filestat) == -1) return;
		m_update = (stat(m_database.c_str(), &cache) == -1 || filestat.st_mtime > cache.st_mtime);
	}

	if(m_update || m_wbfsFS)
	{
		gprintf("Calling list to update filelist\n");
		safe_vector<string> pathlist;
		list.GetPaths(pathlist, containing, path, m_wbfsFS);
		list.GetHeaders(pathlist, *this, m_settingsDir, m_curLanguage);
		
		// Load titles and custom_titles files
		m_loaded = true;
		if(pathlist.size() > 0) Save();
	}
	else
		CCache<T>(*this, m_database, count, LOAD);

	m_loaded = true;
}

template <class T>
string CachedList<T>::make_db_name(string path)
{
	string buffer = path;
	buffer.replace(buffer.find(":/"), 2, "_");
	size_t find = buffer.find("/");
	while(find != string::npos)
	{
		buffer[find] = '_';
		find = buffer.find("/");
	}
	return buffer;
}

template class CachedList<string>;
template class CachedList<dir_discHdr>;
