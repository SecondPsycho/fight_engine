class NewGame {
  public:
    NewGame(KinematicBody2D player1, KinematicBody2D player2, int n) {
      this->statics_count = 0;
      this->statics_max = n;
      this->statics = new KinematicBody2D[this->statics_max];
      this->player1 = player1;
      this->player2 = player2;
    }
    void addStatic(KinematicBody2D newstatic) {
      if (this->statics_count < this->statics_max) {
        this->statics[this->statics_count] = newstatic;
        this->statics_count++;
      } else {
        cout << "Cannot add another static.\n";
      }
    }
    KinematicBody2D* getStatic(int n) {
      if (n < this->statics_max && n >= 0) {
        return &(this->statics[n]);
      }
      cout << "Invalid Static Index.\n";
      return nullptr;
    }
    int getStaticsCount() {
      return this->statics_count;
    }
  private:
    KinematicBody2D player1;
    KinematicBody2D player2;
    KinematicBody2D* statics;
    int statics_count;
    int statics_max;
};