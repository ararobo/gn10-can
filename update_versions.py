import os
import re

# 対象ディレクトリ
target_dir = os.path.join(os.getcwd(), 'include')
drivers_dir = os.path.join(os.getcwd(), 'drivers')

# 更新するバージョン
NEW_VERSION = "0.1.0"

def update_version_in_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()

        # 正規表現: * @version X.X or X.X.X
        # 行頭の空白、*、空白、@version、空白、バージョン番号
        pattern = r'(\*\s*@version\s+)([0-9]+\.[0-9]+(?:\.[0-9]+)?)'

        new_content = re.sub(pattern, f'\\g<1>{NEW_VERSION}', content)

        if content != new_content:
            print(f"Updating {filepath}")
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(new_content)
        else:
            # print(f"No changes in {filepath}")
            pass

    except Exception as e:
        print(f"Error processing {filepath}: {e}")

def walk_and_update(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.hpp') or file.endswith('.h') or file.endswith('.cpp'):
                update_version_in_file(os.path.join(root, file))

if __name__ == '__main__':
    print("Starting version update...")
    walk_and_update(target_dir)
    walk_and_update(drivers_dir)
    print("Version update complete.")
