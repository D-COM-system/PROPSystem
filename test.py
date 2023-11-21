import sys
from dbfread import DBF

# 获取用户输入的 DBF 文件路径
dbf_file_path = sys.argv[1]

# 使用 DBF 类加载 DBF 文件，并指定编码
table = DBF(dbf_file_path, encoding='GBK')

# 获取所有字段名
field_names = list(table.field_names)

# 遍历每一条记录
for record in table:
    # 输出键和值
    for field_name in field_names:
        values = record.get(field_name)
        if values:
            if isinstance(values, list):
                for value in values:
                    print(value)
            else:
                print(values)