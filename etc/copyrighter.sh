#!/bin/bash

for i in `find . -name "*.cpp" -type f`;
do
sed -i "1i\/**\n * Copyright (c) 2019-2020 CHENHAO.\n *\n * This file is part of tinychain.\n *\n * tinychain is free software: you can redistribute it and/or\n * modify it under the terms of the GNU Affero General Public License with\n * additional permissions to the one published by the Free Software\n * Foundation, either version 3 of the License, or (at your option)\n * any later version. For more information see LICENSE.\n *\n * This program is distributed in the hope that it will be useful,\n * but WITHOUT ANY WARRANTY; without even the implied warranty of\n * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n * GNU Affero General Public License for more details.\n *\n * You should have received a copy of the GNU Affero General Public License\n * along with this program. If not, see <http://www.gnu.org/licenses/>.\n */\n " $i
done
