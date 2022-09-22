const int16_t pixelangle[256] = {
	359,		// 0
	357,		// 1
	355,		// 2
	353,		// 3
	351,		// 4
	348,		// 5
	346,		// 6
	344,		// 7
	342,		// 8
	339,		// 9
	337,		// 10
	335,		// 11
	333,		// 12
	330,		// 13
	328,		// 14
	326,		// 15
	323,		// 16
	321,		// 17
	319,		// 18
	316,		// 19
	314,		// 20
	312,		// 21
	309,		// 22
	307,		// 23
	304,		// 24
	302,		// 25
	299,		// 26
	297,		// 27
	294,		// 28
	292,		// 29
	289,		// 30
	287,		// 31
	284,		// 32
	282,		// 33
	279,		// 34
	277,		// 35
	274,		// 36
	272,		// 37
	269,		// 38
	266,		// 39
	264,		// 40
	261,		// 41
	259,		// 42
	256,		// 43
	253,		// 44
	251,		// 45
	248,		// 46
	245,		// 47
	242,		// 48
	240,		// 49
	237,		// 50
	234,		// 51
	231,		// 52
	229,		// 53
	226,		// 54
	223,		// 55
	220,		// 56
	218,		// 57
	215,		// 58
	212,		// 59
	209,		// 60
	206,		// 61
	203,		// 62
	200,		// 63
	197,		// 64
	195,		// 65
	192,		// 66
	189,		// 67
	186,		// 68
	183,		// 69
	180,		// 70
	177,		// 71
	174,		// 72
	171,		// 73
	168,		// 74
	165,		// 75
	162,		// 76
	159,		// 77
	156,		// 78
	153,		// 79
	150,		// 80
	147,		// 81
	144,		// 82
	141,		// 83
	138,		// 84
	134,		// 85
	131,		// 86
	128,		// 87
	125,		// 88
	122,		// 89
	119,		// 90
	116,		// 91
	113,		// 92
	109,		// 93
	106,		// 94
	103,		// 95
	100,		// 96
	97,		// 97
	94,		// 98
	90,		// 99
	87,		// 100
	84,		// 101
	81,		// 102
	78,		// 103
	74,		// 104
	71,		// 105
	68,		// 106
	65,		// 107
	61,		// 108
	58,		// 109
	55,		// 110
	52,		// 111
	48,		// 112
	45,		// 113
	42,		// 114
	39,		// 115
	35,		// 116
	32,		// 117
	29,		// 118
	26,		// 119
	22,		// 120
	19,		// 121
	16,		// 122
	13,		// 123
	9,		// 124
	6,		// 125
	3,		// 126
	0,		// 127
	0,		// 128
	-3,		// 129
	-6,		// 130
	-9,		// 131
	-13,		// 132
	-16,		// 133
	-19,		// 134
	-22,		// 135
	-26,		// 136
	-29,		// 137
	-32,		// 138
	-35,		// 139
	-39,		// 140
	-42,		// 141
	-45,		// 142
	-48,		// 143
	-52,		// 144
	-55,		// 145
	-58,		// 146
	-61,		// 147
	-65,		// 148
	-68,		// 149
	-71,		// 150
	-74,		// 151
	-78,		// 152
	-81,		// 153
	-84,		// 154
	-87,		// 155
	-90,		// 156
	-94,		// 157
	-97,		// 158
	-100,		// 159
	-103,		// 160
	-106,		// 161
	-109,		// 162
	-113,		// 163
	-116,		// 164
	-119,		// 165
	-122,		// 166
	-125,		// 167
	-128,		// 168
	-131,		// 169
	-134,		// 170
	-138,		// 171
	-141,		// 172
	-144,		// 173
	-147,		// 174
	-150,		// 175
	-153,		// 176
	-156,		// 177
	-159,		// 178
	-162,		// 179
	-165,		// 180
	-168,		// 181
	-171,		// 182
	-174,		// 183
	-177,		// 184
	-180,		// 185
	-183,		// 186
	-186,		// 187
	-189,		// 188
	-192,		// 189
	-195,		// 190
	-197,		// 191
	-200,		// 192
	-203,		// 193
	-206,		// 194
	-209,		// 195
	-212,		// 196
	-215,		// 197
	-218,		// 198
	-220,		// 199
	-223,		// 200
	-226,		// 201
	-229,		// 202
	-231,		// 203
	-234,		// 204
	-237,		// 205
	-240,		// 206
	-242,		// 207
	-245,		// 208
	-248,		// 209
	-251,		// 210
	-253,		// 211
	-256,		// 212
	-259,		// 213
	-261,		// 214
	-264,		// 215
	-266,		// 216
	-269,		// 217
	-272,		// 218
	-274,		// 219
	-277,		// 220
	-279,		// 221
	-282,		// 222
	-284,		// 223
	-287,		// 224
	-289,		// 225
	-292,		// 226
	-294,		// 227
	-297,		// 228
	-299,		// 229
	-302,		// 230
	-304,		// 231
	-307,		// 232
	-309,		// 233
	-312,		// 234
	-314,		// 235
	-316,		// 236
	-319,		// 237
	-321,		// 238
	-323,		// 239
	-326,		// 240
	-328,		// 241
	-330,		// 242
	-333,		// 243
	-335,		// 244
	-337,		// 245
	-339,		// 246
	-342,		// 247
	-344,		// 248
	-346,		// 249
	-348,		// 250
	-351,		// 251
	-353,		// 252
	-355,		// 253
	-357,		// 254
	-359,		// 255
};
