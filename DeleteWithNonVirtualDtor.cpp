class IPluginImpl {

  public:
	IPluginImpl() {
	}
};

class TPlugin : public IPluginImpl {
  public:
	TPlugin() {
	}

	void Create() {
		IPluginImpl* t = new TPlugin();
		delete t;
		try {
		}
		catch (...) {
		}
	}
};
