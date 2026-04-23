void sleepSort(std::vector<int>& v) {
    std::mutex mtx;
    std::vector<int> r = v;
    v.clear();

    // Group duplicates: each unique value gets ONE thread
    // that pushes the value count-many times
    std::unordered_map<int, int> freq;
    for (int e : r) {
        if (e < 0)
            throw std::range_error("Sleep sort cannot handle negative numbers: "s + std::to_string(e));
        freq[e]++;
    }

    // Build all threads FIRST, then launch them all at once
    std::vector<std::thread> ths;
    ths.reserve(freq.size());

    int minVal = *std::min_element(r.begin(), r.end());
    int maxVal = *std::max_element(r.begin(), r.end());
    std::cout << "SleepSort debug - min: " << minVal << " max: " << maxVal << " unique: " << freq.size() << std::endl;

    for (auto& [val, count] : freq) {
        ths.push_back(std::thread([val, count, &v, &mtx]() {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds((val + 1) * 100));
                std::lock_guard<std::mutex> lock(mtx);
                for (int i = 0; i < count; i++)
                    v.push_back(val);
            } catch (const std::exception& ex) {
                std::cerr << "Thread exception for val=" << val << ": " << ex.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown thread exception for val=" << val << std::endl;
            }
        }));
    }

    for (auto& t : ths)
        if (t.joinable()) t.join();

    std::cout << "SleepSort debug - output size: " << v.size() << std::endl;
}