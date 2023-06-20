add_rules("mode.debug")
add_requires('asio', 'cryptopp', 'zlib')

target("MinePlusPlus")
    set_warnings("everything")
    set_languages("cxx20")
    set_optimize("none")
    add_files('Src/*.cpp', 'Src/Network/*.cpp', 'Src/Utils/*.cpp')
    add_includedirs('Include', 'Include/Network', 'Include/Threading')
    add_packages('asio', 'cryptopp', 'zlib')
    after_build(function(target)
        print("Copying maps to build directory...")
        local binaryPath = "$(buildir)/$(plat)/$(arch)/$(mode)"
        os.mkdir(binaryPath .. "/Maps")
        os.cp("Maps/**", binaryPath .. "/Maps")
        print("Copying maps to build directory... Done !")
    end)

