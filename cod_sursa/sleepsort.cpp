void sleepSort(std::vector<int>& v) {
    std::mutex mtx;
    std::vector<int> r = v;
    v.clear();

    std::unordered_map<int, int> freq;
    for (int e : r) {
        if (e < 0)
            throw std::range_error(
                "Sleep sort cannot handle negative numbers: " + 
                std::to_string(e));
        freq[e]++;
    }

    std::vector<std::thread> ths;
    ths.reserve(freq.size());

    for (auto& [val, count] : freq) {
        ths.push_back(std::thread([val, count, &v, &mtx]() {
            std::this_thread::sleep_for(
                std::chrono::milliseconds((val + 1) * 100));
            std::lock_guard<std::mutex> lock(mtx);
            for (int i = 0; i < count; i++)
                v.push_back(val);
        }));
    }

    for (auto& t : ths)
        if (t.joinable()) t.join();
}