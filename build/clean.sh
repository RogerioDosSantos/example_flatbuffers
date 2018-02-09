# Exits on any non-zero status.
trap 'exit' ERR
set -E

# Get variables based on the configuration
cd "$(dirname "$0")"
bash_dir=$(pwd)

echo " "
echo "*** Cleanning: ${bash_dir}" 
rm -r */
rm -r ${bash_dir}/../stage/
