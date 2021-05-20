class mutex {
public:
    mutex() : val(0) { }
 
    void lock() {
        while (cmpxchg(val, 0, 1) != 0) {
            // Changing from 0 to 1 means that we have acquired the lock.
        }
    }
 
    void unlock() {
        atomic_dec(val);
    }
 
private:
    int val;
}

