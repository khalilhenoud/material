#if !defined(MATERIAL_API)
	#define MATERIAL_API /* NOTHING */

	#if defined(WIN32) || defined(WIN64)
		#undef MATERIAL_API
		#if defined(material_EXPORTS)
			#define MATERIAL_API __declspec(dllexport)
		#else
			#define MATERIAL_API __declspec(dllimport)
		#endif
	#endif // defined(WIN32) || defined(WIN64)

#endif // !defined(MATERIAL_API)

