void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m->menuItems() == nullptr)
		return;

	vector<MenuItem*>::const_iterator itr = m->menuItems()->begin();
	for (; itr < m->menuItems()->end(); itr++)
	{
		if (path.empty())
		{
			cout << path + (*itr)->name() << endl;
			listAll(*itr, path + (*itr)->name());
		}
		else
		{
			cout << path + '/' + (*itr)->name() << endl;
			listAll(*itr, path + '/' + (*itr)->name());
		}
	}

}

