// Takes in regex and generates NFA for it
#include <map>
#include <vector>
#include <string>
#include <iostream>

template <typename T>
class NFANode
{
private:
    bool isFinal = false;
    std::map<T, std::vector<NFANode<T> *>> stateTransition;
    int nodeId;

public:
    template <typename>
    friend class NFA;
    inline bool is_final() { return isFinal; };

    inline const std::vector<NFANode<T> *> &operator[](T c)
    {
        return stateTransition[c];
    }

    inline void add_transition(T c, NFANode<T> *n)
    {
        stateTransition[c].push_back(n);
    }

    NFANode(int nodeId) : nodeId(nodeId){};
};

template <typename T>
class NFA
{
private:
    int idCounter = 0;
    std::vector<T> alphabet;
    std::map<int, NFANode<T> *> nodes;
    std::vector<T> regex;
    T asterate;

public:
    NFA(std::vector<T> &alphabet, std::vector<T> &regex, T asterate) : alphabet(std::move(alphabet)), regex(std::move(regex)), asterate(asterate)
    {
        nodes[idCounter] = new NFANode<T>(idCounter);
        for (int i = 0; i < this->regex.size(); i++)
        {
            gen_node(idCounter);
        }
        nodes[idCounter]->isFinal = true;
    };

    void gen_node(int &index)
    {
        NFANode<T> *newNode = new NFANode<T>(index + 1);
        NFANode<T> *lastNode = nodes[index];
        if (regex[index] == asterate)
        {
            free(newNode);
            lastNode->add_transition(regex[index], lastNode);
        }
        else
        {
            lastNode->add_transition(regex[index], newNode);
            nodes[++index] = newNode;
        }
    }

    bool is_valid_expr(const std::vector<T> &expr)
    {
        std::vector<NFANode<T> *> currStates;
        currStates.push_back(nodes[0]);
        for (T c : expr)
        {
            std::vector<NFANode<T> *> nextStates;
            for (auto s : currStates)
            {
                for (auto x : (*s)[c])
                {
                    if (x != NULL)
                        nextStates.push_back(x);
                }
                for (auto x : (*s)[asterate])
                {
                    if (x != NULL)
                        nextStates.push_back(x);
                }
            }

            currStates = nextStates;
        }
        for (auto s : currStates)
            if (s->is_final())
                return true;
        return false;
    }
};

int main()
{

    std::vector<char> alphabet = {'a', 'b', 'c'};
    std::vector<char> regex = {'a', '*'};

    NFA<char> *nfa = new NFA<char>(alphabet, regex, '*');

    std::cout << nfa->is_valid_expr({'a', 'a', 'b'}) << std::endl;

    return 0;
}