#ifndef MEIPURU_MEIPURUEXPORT_H
#define MEIPURU_MEIPURUEXPORT_H

#ifdef MEIPURU_LIB
#if (defined(_WIN32) || defined(_WIN64))
#define MEIPURU_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define MEIPURU_EXPORT __attribute__ ((visibility("default")))
#else
#define MEIPURU_EXPORT
#endif // Compiler
#endif // MEIPURU_LIB

#endif //MEIPURU_MEIPURUEXPORT_H
