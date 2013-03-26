#!/bin/dash

test $(ls -a | wc -l) != 2 && exit 1

set -e

xml_dir=$(dirname $0)/xml
for f in $xml_dir/*.*
do
	test ${f##*.} != xslt && xsltproc $xml_dir/patch.xslt $f > ${f##*/}
done

wsdl_list=$(ls *.wsdl | tr "\n" " " | sed s/rw-2.wsdl// | sed s/bw-2.wsdl//)
src_type=

mkdir src
wsdl2h -o /dev/stdout -t $(dirname $0)/typemap.dat \
		$wsdl_list 2>/dev/null | soapcpp2 -I /usr/share/gsoap/import -inxL$1 -ponvif -f100 -dsrc
sed -i 's/onvif_namespaces/namespaces/' src/onvif.nsmap

