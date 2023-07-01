#pragma once

#ifndef DELTAS_CHESS_CODE_UTILS_H
#define DELTAS_CHESS_CODE_UTILS_H

#define NAMESPACE_DDDELTA_START namespace DDDelta {
#define NAMESPACE_DDDELTA_END }

#ifdef DDDELTA_SETTING_DEBUG
#define STDARRAY_GET(arr, x) (arr.at(x))
#else
#define STDARRAY_GET(arr, x) (arr[x])
#endif // DDDELTA_SETTING_DEBUG


#endif // DELTAS_CHESS_CODE_UTILS_H
