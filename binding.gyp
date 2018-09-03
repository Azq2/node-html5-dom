{
	"targets": [{
		"target_name":		"html5-dom",
		"sources":			[
			"src/addon.cpp", 
			"src/Parser.cpp", 
			"src/Tree.cpp", 
			"src/Utils.cpp", 
			
			"src/modest/modest_myurl.c", 
			"src/modest/modest_mycss.c", 
			"src/modest/modest_config.h", 
			"src/modest/modest_myencoding.c", 
			"src/modest/modest_mycore.c", 
			"src/modest/modest_myfont.c", 
			"src/modest/modest_modest.c", 
			"src/modest/modest_myhtml.c", 
			"src/modest/modest_myport.c"
		], 
		"include_dirs":		[
			"<!(node -e \"require('nan')\")", 
			"third_party/modest/include", 
			"."
		]
	}]
}
