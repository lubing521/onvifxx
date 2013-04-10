#!/bin/dash

test $(ls -a | wc -l) -gt 6 && exit 1

set -e

xml_dir=$(dirname $0)/xml
for f in $xml_dir/*.*
do
	test ${f##*.} != xslt && xsltproc $xml_dir/patch.xslt $f > ${f##*/}
done

wsdl_list=$(ls *.wsdl | tr "\n" " " | sed s/rw-2.wsdl// | sed s/bw-2.wsdl//)
echo $wsdl_list
#wsdl_list="${wsdl_list}"

mkdir src
ln -s /usr/share/gsoap/import/stlvector.h stlvector.h
ln -s /usr/share/gsoap/import/xop.h xop.h
ln -s /usr/share/gsoap/import/wsdd10.h wsdd.h
ln -s /usr/share/gsoap/import/wsdx.h wsdx.h
ln -s /usr/share/gsoap/import/wsa.h wsa.h

wsdl2h -xP -o /dev/stdout -t $(dirname $0)/typemap.dat \
		${wsdl_list} 2>/dev/null | soapcpp2  -inxL$1 -ponvif -f100 -dsrc
sed -i 's/onvif_namespaces/namespaces/' src/onvif.nsmap

