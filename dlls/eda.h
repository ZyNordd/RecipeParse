#include <fstream>
#include <vector>
#include <sstream>
/*!
    \defgroup eda Парсер сайта eda.ru
    \brief Данная библиотека предназначена для парсинга сайта eda.ru
*/

#ifdef _WIN32
///@{
    /*! \brief Функция для парсинга ингредиентов
         \param htmlcode исходный код страницы
          \returns Ингредиенты, их количество и количество порций
    */
extern "C" __declspec(dllexport) std::string ingredients(std::string htmlcode);

    /*! \brief Функция для парсинга названия блюда
         \param htmlcode исходный код страницы
          \returns Название блюда
    */
extern "C" __declspec(dllexport) std::string name(std::string htmlcode);

/*! \brief Функция для парсинга калорий
         \param htmlcode исходный код страницы
          \returns Калорийность
    */
extern "C" __declspec(dllexport) int calories(std::string htmlcode);

/*! \brief Функция для парсинга белка
         \param htmlcode исходный код страницы
          \returns Количество белка в граммах
    */
extern "C" __declspec(dllexport) int protein(std::string htmlcode);

/*! \brief Функция для парсинга жиров
         \param htmlcode исходный код страницы
          \returns Количество жиров в граммах
    */
extern "C" __declspec(dllexport) int fat(std::string htmlcode);

/*! \brief Функция для парсинга углеводов
         \param htmlcode исходный код страницы
          \returns Количество углеводов в граммах
    */
extern "C" __declspec(dllexport) int carbohydrate(std::string htmlcode);

    /*! \brief Функция для парсинга рецепта
         \param htmlcode исходный код страницы
          \returns Текст рецепта
    */
extern "C" __declspec(dllexport) std::string recipe(std::string htmlcode);

    /*! \brief Функция для парсинга ссылок на рецепт
        \param htmlcode исходный код страницы
         \returns Ссылка на рецепт
   */
extern "C" __declspec(dllexport) std::string URLs(std::string htmlcode);
///@}
#else
///@{
    /*! \brief Функция для парсинга ингредиентов
         \param htmlcode исходный код страницы
          \returns Ингредиенты, их количество и количество порций
    */
extern "C" std::string ingredients(std::string htmlcode);

/*! \brief Функция для парсинга названия блюда
     \param htmlcode исходный код страницы
      \returns Название блюда
*/
extern "C" std::string name(std::string htmlcode);

/*! \brief Функция для парсинга калорий
         \param htmlcode исходный код страницы
          \returns Калорийность
    */
extern "C" int calories(std::string htmlcode);

/*! \brief Функция для парсинга белка
         \param htmlcode исходный код страницы
          \returns Количество белка в граммах
    */
extern "C" int protein(std::string htmlcode);

/*! \brief Функция для парсинга жиров
         \param htmlcode исходный код страницы
          \returns Количество жиров в граммах
    */
extern "C" int fat(std::string htmlcode);

/*! \brief Функция для парсинга углеводов
         \param htmlcode исходный код страницы
          \returns Количество углеводов в граммах
    */
extern "C" int carbohydrate(std::string htmlcode);

/*! \brief Функция для парсинга рецепта
     \param htmlcode исходный код страницы
      \returns Текст рецепта
*/
extern "C" std::string recipe(std::string htmlcode);

/*! \brief Функция для парсинга ссылок на рецепт
    \param htmlcode исходный код страницы
     \returns Ссылка на рецепт
*/
extern "C" std::string URLs(std::string htmlcode);
///@}

#endif