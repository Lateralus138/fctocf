//////////////////////////////////////////////////
// 'F/C To C/f' - Convert Fahrenheit to Celsius //
// and vice versa.                              //
// © 2022 Ian Pride - New Pride Software        //
// /Services                                    //
//////////////////////////////////////////////////
//////////////////////////////////////////////////
// Force no bound limit for min and max float   //
// in given temperature query. A bounds check   //
// seems redundant and you can't always         //
// correctly check and it seems interesting to  //
// test the bounds of the imagination of what a //
// given temperature might be.                  //
//////////////////////////////////////////////////

#include <iostream>
#include <Windows.h>
#include "functions.h"
#include <regex>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>

int main(int argc, char* argv[])
{
  const
    std::regex
      RGXHELP             ("^/([hH]|[hH][eE][lL][pP]|[\?])$"),
      RGXCELS             ("^/([cC]|[cC][eE][lL][sS][iI][uU][sS])$"),
      RGXFAHR             ("^/([fF]|[fF][aA][hH][rR][eE][nN][hH][eE][iI][tT])$"),
      RGXSHOWTYPE         ("^/([sS]|[sS][hH][oO][wW][tT][yY][pP][eE])$"),
      RGXTEMP             ("^/([tT]|[Tt][eE][mM][pP])$"),
      RGXPRECISION        ("^/([pP]|[pP][rR][eE][cC][iI][sS][iI][oO][nN])$"),
      RGXINT              ("^[0-9]*$"),
      RGXSIGNEDINTORLFOAT ("^[+-]?([0-9]*[.])?[0-9]+$");
  std::string
    queryString,
    finalResultString,
    finalResultTypeString;
  bool
    fromPipe = false,
    isResultTempCelsius = true,
    showType = false;
  int
    precision = 4;
  const std::vector <std::string> BOX =
  {
    u8"\u250C", u8"\u2500", u8"\u2510", u8"\u2502",
    u8"\u2514", u8"\u2518", u8"\u251C", u8"\u2524"
  };
  const std::string CR = u8"\u00A9";
  std::string box_line;
  for (auto i = 0; i < 48; ++i)
  {
    box_line += BOX[1];
  }

  {
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD handleType = GetFileType(inputHandle);
    switch (handleType)
    {
      case FILE_TYPE_PIPE:
        std::string input;
        while (std::getline(std::cin, input))
        {
          if (queryString.empty())
          {
            fromPipe = true;
            queryString = input;
          }
          else break;
        }
        break;
    }
  }

  if (argc > 1)
  {
    std::string thisArg;
    bool skip = false;
    for (int index = 1; index < argc; index++)
    {
      if (skip)
      {
        skip = false;
        continue;
      }
      thisArg = argv[index];
      if (!thisArg.empty())
      {
        if (std::regex_match(thisArg, RGXHELP))
        {
          std::cout <<
            '\n' +
            BOX[0] + box_line + BOX[2] + '\n' +
            BOX[3] + "                   F/C To C/F                   " + BOX[3] + '\n' +
            BOX[6] + box_line + BOX[7] + '\n' +
            BOX[3] + " Convert Fahrenheit to Celsius and vice versa.  " + BOX[3] + '\n' +
            BOX[3] + " If a signed integer or double is passed from a " + BOX[3] + '\n' +
            BOX[3] + " pipe it overrides any other TEMPERATURE passed " + BOX[3] + '\n' +
            BOX[3] + " to this program.                               " + BOX[3] + '\n' +
            BOX[6] + box_line + BOX[7] + '\n' +
            BOX[3] + " fctocf [/H] [/F] [/C] [/P INT] [/T TEMP] TEMP  " + BOX[3] + '\n' +
            BOX[6] + box_line + BOX[7] + '\n' +
            BOX[3] + " /h,/help        This help screen.              " + BOX[3] + '\n' +
            BOX[3] + " /c,/celsius     Retrieve conversion in         " + BOX[3] + '\n' +
            BOX[3] + "                 Celsius; this is the default.  " + BOX[3] + '\n' +
            BOX[3] + " /f,/fahrenheit  Retrieve conversion in         " + BOX[3] + '\n' +
            BOX[3] + "                 Fahrenheit.                    " + BOX[3] + '\n' +
            BOX[3] + " /s,/showtype    Display 'C' or 'F' with        " + BOX[3] + '\n' +
            BOX[3] + "                 the conversion.                " + BOX[3] + '\n' +
            BOX[3] + " /t,/temp        Temperature to convert. This   " + BOX[3] + '\n' +
            BOX[3] + "                 is to set a default in case    " + BOX[3] + '\n' +
            BOX[3] + "                 one is not passed to the       " + BOX[3] + '\n' +
            BOX[3] + "                 via standard input or a pipe.  " + BOX[3] + '\n' +
            BOX[3] + " /p,/precision   Digit precision of conversion. " + BOX[3] + '\n' +
            BOX[3] + "                 Defaults to 4 with a minimum   " + BOX[3] + '\n' +
            BOX[3] + "                 of 2.                          " + BOX[3] + '\n' +
            BOX[6] + box_line + BOX[7] + '\n' +
            BOX[3] + " " + CR + " 2022 Ian Pride - New Pride Software/Services " + BOX[3] + '\n' +
            BOX[4] + box_line + BOX[5] + "\n\n";
          return 0;
        }
        if (std::regex_match(thisArg, RGXCELS))
        {
          isResultTempCelsius = true;
          continue;
        }
        if (std::regex_match(thisArg, RGXFAHR))
        {
          isResultTempCelsius = false;
          continue;
        }
        if (std::regex_match(thisArg, RGXSHOWTYPE))
        {
          showType = true;
          continue;
        }
        if ((!fromPipe) && std::regex_match(thisArg, RGXTEMP))
        {
          if ((index + 1) < argc)
          {
            std::string nextArg = argv[index + 1];
            if (nextArg.empty())
            {
              std::cerr << "Argument provided for '" << thisArg << "' is empty.\n";
              return 2;
            }
            skip = true;
            queryString = nextArg;
            continue;
          }
          else
          {
            std::cerr << "No argument provided for '" << thisArg << "'.\n";
            return 1;
          }
        }
        if (std::regex_match(thisArg, RGXPRECISION))
        {
          if ((index + 1) < argc)
          {
            std::string nextArg = argv[index + 1];
            if (nextArg.empty())
            {
              std::cerr << "Argument provided for '" << thisArg << "' is empty.\n";
              return 4;
            }
            if (!std::regex_match(nextArg, RGXINT))
            {
              std::cerr << "Argument provided for '" << thisArg << "' is not an integer.\n";
              return 5;
            }
            skip = true;
            precision = std::stoi(nextArg);
            if (precision < 2)
            {
              std::cerr << "Integer provided for '" << thisArg << "' is less than the minimum: 2.\n";
              return 6;
            }
            continue;
          }
          else
          {
            std::cerr << "No argument provided for '" << thisArg << "'.\n";
            return 3;
          }
        }
        if ((!fromPipe) && queryString.empty())
        {
          queryString = thisArg;
        }
      }
    }
  }
  if (queryString.empty())
  {
    std::cerr
      << "No temperature provided.\n"
      "An integer or decimal must be provided by standard "
      "input, from pipe, or passed to the -t parameter.\n";
    return 7;
  }
  if (std::regex_match(queryString, RGXSIGNEDINTORLFOAT))
  {
    double queryFloat = std::stof(queryString);
    double resultFloat = [queryFloat, isResultTempCelsius]()
    {
      return
        (
          isResultTempCelsius
          ? FahrenheitToCelsius(queryFloat)
          : CelsiusToFahrenheit(queryFloat)
          );
    }();
    std::stringstream str_conversion;
    str_conversion << std::setprecision(precision) << resultFloat;
    finalResultString = str_conversion.str();
  }
  else
  {
    std::cerr << "Provided temperature is not a valid integer or decimal value.\n";
    return 8;
  }
  finalResultTypeString = [showType, isResultTempCelsius]()
  {
    return (showType ? (isResultTempCelsius ? "C" : "F") : "");
  }();
  std::cout << finalResultString + finalResultTypeString << '\n';
}
