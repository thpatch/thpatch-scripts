for f in */; do
	f=$(echo $f | tr -d /)
	if [ -f $f/spellcard.cv1 ]; then
		./cv1_to_wiki.py $f/spellcard.cv1 $f > $f/mw.txt
	fi
done
