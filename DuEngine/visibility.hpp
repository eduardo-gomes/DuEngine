#ifndef DUENG_RELEASE
#define LOGDEBUG(X) logger::dbug(X)
#define LOGPTDEBUG() LOGDEBUG(std::string(__PRETTY_FUNCTION__) + std::string(" at " __FILE__ ":") + std::to_string(__LINE__))
#else
#endif


#ifndef DUENG_DLL_MACROS
#define DUENG_DLL_MACROS
#ifdef _WIN32
	#define DUENG_DLL_IMPORT __declspec(dllimport)
	#define DUENG_DLL_EXPORT __declspec(dllexport)
	#define DUENG_DLL_LOCAL
#else
	#if __GNUC__ >= 4
		#define DUENG_DLL_IMPORT __attribute__ ((visibility ("default")))
		#define DUENG_DLL_EXPORT __attribute__ ((visibility ("default")))
		#define DUENG_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
	#else
		#define DUENG_DLL_IMPORT
		#define DUENG_DLL_EXPORT
		#define DUENG_DLL_LOCAL
	#endif
#endif


#ifdef DUENG_DLL
	#ifdef DUENG_DLL_BUILD
		#define DUENGEXT DUENG_DLL_EXPORT
	#else
		#define DUENGEXT DUENG_DLL_IMPORT
	#endif

	#define DUENGINT DUENG_DLL_LOCAL
#else // static lib
	#define DUENGEXT
	#define DUENGINT
#endif	//Define DUENG_DLL
#endif	//Define DUENG_DLL_MACROS