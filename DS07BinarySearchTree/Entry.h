template <typename K, typename V>
struct Entry
{
    K key;
    V value;
    Entry(K k = K(), V v = V()) : key(k), value(v){};           //默认构造函数
    Entry(Entry<K, V> const &e) : key(e.key), value(e.value){}; //克隆
    //比较器与判等器
    //实际上，比较和判等的就是词条的key;以后我们不再严格区分词条和它的key
    bool operator<(Entry<K, V> const &e) { return key < e.key; }
    bool operator>(Entry<K, V> const &e) { return key > e.key; }
    bool operator==(Entry<K, V> const &e) { return key == e.key; }
    bool operator!=(Entry<K, V> const &e) { return key != e.key; }
};
