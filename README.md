# Title – Portfolio Creator

## Functionality overview

This program creates an efficient stock portfolio using Modern Portfolio theory. Based on stocks selected by the user, it calculates the mix of stocks that will give you the maximum return for a given level of risk. Risk in this context is defined as standard deviation.

## Installation & Usage

1. Downloading & Building the Libraries
	- Download wxWidgets GUI Library – extract files, navigate to build > msw > open “wx_vc17.sln”
		- In the solution explorer, select all files excluding _custom_build > right click “properties” > C/C++ > Code generation > Runtime Library = “Multi-Threaded (/MT)” > click apply > click ok > close Property page
		- In the ribbon at the top of the Visual Stuido interface click “Build” > “Batch Build” > select only the static, x86, & release files > clicked “build”
		- Make an environment variable “WXWIN” pointing to the root of the directory
	- Build CPR – linear algebra library
		- Make an environment variable “EIGEN_ROOT” pointing to root of directory
		- Using vcpkg, type in ./vcpkg install cpr:x86-windows-static
2. Visual Studio Configuration
	- Open the solution file of the program in Visual Studio then, click Project > Properties 
		- C/C++ > Runtime Library = “Multi-Threaded (/MT)”
		- Added the filepaths/lib names as pictured below
		- ![](https://github.com/MarkHohertz/portfolioCreator/blob/main/pcReadMe1.png)
		- ![](https://github.com/MarkHohertz/portfolioCreator/blob/main/pcReadme2.png)
		- ![](https://github.com/MarkHohertz/portfolioCreator/blob/main/pcReadme3.png)

## Known Issues

	- Using getters/setters vs. just including attributes as public. Some standardization needed in this project.
	- Anti-patterns – The frontier class is quite large. It was going to be considerably larger, but I recognized the instance of it was a god object. I attempted to split some of the functionality off into the outputstats class. Outputstats has the feel of a poltergeist though.
	- Some methods of other classes are also too long. I wasn’t sure how much value there was in splitting apart the frame class constructor.
	- Bug avoidance – While there is some data validation, there are still a some sequences a user can input that result in analysis being run on empty or flawed datasets. 
	- One weaknesses of the academic paper I used to create the program is that it does not limit the number or magnitude of short positions. The result is that the program sometimes produces portfolios with short positions that are a very high (sometimes impossibly high) % of the initial investment.

## Acknowledgements

	- Without the initial theory created by Harry Markowitz and the subequent [analytical derivation](https://www.researchgate.net/publication/38006987_An_Analytic_Derivation_of_the_Efficient_Portfolio_Frontier) by Robert C. Merton, this project would have been impossible
	- The moderators on the wxWidgets forum answer questions daily. Without seeing their answers to other user's issues, this would have taken far longer to finish.

Mark A. Hohertz
