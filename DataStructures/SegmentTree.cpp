#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <functional>

template<typename Func>
concept Function = std::is_function<Func>::value;

// classic SegmentTree data structure
// heavy realisation, that based on a graph
// that works ONLY with associative functions
template<typename T, Function F>
class SegmentTree {
private:

    struct Node {
        T value_;

        Node* leftChild_ = nullptr;
        Node* rightChild_ = nullptr;

        Node() = default;
        Node(T& value) : value_(value) {}
    };

    // T * N + size_t * N memory
    Node* root_;
    // T * N of memory
    // obv, i don't need it
    std::vector<T> initialArray_;

public:
    const std::function<T(T, T)> function_;
    const T neutralElement_;

private:
    Node* initNode(size_t leftEnd, size_t rightEnd) {
        if (leftEnd + 1 == rightEnd) {
            Node* treeLeave = new Node(initialArray_[leftEnd]);
            return treeLeave;
        }
        else {
            size_t middle = (leftEnd + rightEnd) / 2;

            Node* pNode = new Node();
            pNode->leftChild_ = this->initNode(leftEnd, middle);
            pNode->rightChild_ = this->initNode(middle, rightEnd);

            pNode->value_ = function_(pNode->leftChild_->value_, pNode->rightChild_->value_);
            return pNode;
        }
    }

    void deleteDFS(Node* toDeleteNode) {
        if (toDeleteNode == nullptr)
            return;
        deleteDFS(toDeleteNode->leftChild_);
        deleteDFS(toDeleteNode->rightChild_);

        delete toDeleteNode;
    }

    T applyFunc(Node* currentNode, size_t leftEnd, size_t rightEnd,
               size_t queryLeftEnd, size_t queryRightEnd) {
        
        if (currentNode == nullptr) {
            return neutralElement_;
        }
        
        if (leftEnd >= queryLeftEnd && rightEnd <= queryRightEnd)
            return currentNode->value_;

        if (std::max(leftEnd, queryLeftEnd)
            >= std::min(rightEnd, queryRightEnd))
            return neutralElement_;

        size_t middle = (leftEnd + rightEnd) / 2;
        auto lhs = applyFunc(currentNode->leftChild_, leftEnd, middle,
                            queryLeftEnd, queryRightEnd);
        auto rhs = applyFunc(currentNode->rightChild_, middle, rightEnd,
                            queryLeftEnd, queryRightEnd);
        return function_(lhs, rhs);
    }

    void changeElement(Node* currentNode, T newValue, size_t index,
                       size_t leftEnd, size_t rightEnd) {
        if (leftEnd == index && leftEnd + 1 == rightEnd){
            currentNode->value_ = newValue;
            return;
        }

        auto lhs = currentNode->leftChild_;
        auto rhs = currentNode->rightChild_;
        
        // check if children exist
        if (lhs == nullptr)
            return;

        size_t middle = (leftEnd + rightEnd) / 2;

        if (index < middle) {
            changeElement(lhs, newValue, index, leftEnd, middle);
        }
        else
            changeElement(rhs, newValue, index, middle, rightEnd);
        
        currentNode->value_ = function_(lhs->value_, rhs->value_);
    }

public:
    SegmentTree() = delete;
    SegmentTree(std::vector<T> &array, F function, T neutral)
    : initialArray_(array), function_(function), neutralElement_(neutral) {
        this->root_ = initNode(0, array.size());
    }

    ~SegmentTree() {
        this->deleteDFS(this->root_);
    }

    T getFunc(size_t queryLeftEnd, size_t queryRightEnd) {
        return this->applyFunc(root_, 0, initialArray_.size(), queryLeftEnd, queryRightEnd);
    }

    void changeValue(T newValue, size_t index) {
        this->changeElement(root_, newValue, index, 0, initialArray_.size());
    }
};

int sum(int x, int y) {
    return x + y;
}

int intMax(int x, int y) {
    return (x > y) ? x : y;
}

std::string strMax(std::string x, std::string y) {
    return (x > y) ? x : y;
}

int intMul(int x, int y) {
    return x * y;
}

template<typename T, Function F>
bool test(SegmentTree<T, F>& toTest, std::vector<T>& initArr) {
    auto function = toTest.function_;
    T neutral = toTest.neutralElement_;
    size_t n = initArr.size();

    for (size_t j = 1; j < n; ++j)
        for (size_t i = 0; i < n - j + 1; ++i) {
            T correct = neutral;
            for (size_t k = 0; k < j; ++k)
                correct = function(correct, initArr[i + k]);

            if (correct != toTest.getFunc(i, i + j))
                return false;
        }

    return true;
}

int main() {
    {
        std::vector<int> testArr{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        SegmentTree<int, decltype(sum)> sumTree(testArr, sum, 0);
        bool getFlag = test<int, decltype(sum)>(sumTree, testArr);
        assert(getFlag);

        bool changeFlag = test<int, decltype(sum)>(sumTree, testArr);
        assert(changeFlag);
        std::cout << "Passed 1 test" << std::endl;
    }

    {
        std::vector<std::string> testArr{"a", "d", "b", "c", "cd", "ab", "ac", "bc"};
        SegmentTree<std::string, decltype(strMax)> maxTree(testArr, strMax, "\0");
        bool getFlag = test<std::string, decltype(strMax)>(maxTree, testArr);
        assert(getFlag);

        testArr[4] = "hhh";
        testArr[5] = "vpert";

        maxTree.changeValue("hhh", 4);
        maxTree.changeValue("vpert", 5);

        bool changeFlag = test<std::string, decltype(strMax)>(maxTree, testArr);
        assert(changeFlag);
        std::cout << "Passed 2 test" << std::endl;
    }
 
    std::vector<int> testArr{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};   
    // SegmentTree<int, decltype(intMax)> maxTree(testArr, intMax, -1);
    // maxTree.changeValue(90, 0);
    // std::cout << maxTree.getFunc(0, 5) << std::endl;
    // maxTree.changeValue(101, 5);
    // std::cout << maxTree.getFunc(0, 11) << std::endl;

    SegmentTree<int, decltype(intMul)> multTree(testArr, intMul, 1);
    std::cout << multTree.getFunc(0, testArr.size()) << std::endl;
    multTree.changeValue(1, 9);
    std::cout << multTree.getFunc(0, testArr.size()) << std::endl;
    std::cout << multTree.getFunc(9, 11) << std::endl;

    return 0;
}