// 수업 7-5 실습: 스마트 포인터로 게임 오브젝트 관리
// 날짜: 2026-04-10
//
// 설계:
//   - Player        → shared_ptr (월드가 소유, Monster가 weak_ptr로 참조)
//   - Monster       → shared_ptr (여러 시스템이 공유)
//   - Monster.target→ weak_ptr   (순환 참조 방지)
//   - Texture       → shared_ptr (여러 몬스터가 공유)
//
// 주의: weak_ptr은 shared_ptr에서만 만들 수 있다.
//      그래서 Player를 shared_ptr로 둠.

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// ============ Texture ============
class Texture
{
public:
    Texture(const std::string& name) : m_Name(name)
    {
        std::cout << "[Texture] " << m_Name << " 로드\n";
    }
    ~Texture()
    {
        std::cout << "[Texture] " << m_Name << " 해제\n";
    }

    std::string GetName() const { return m_Name; }

private:
    std::string m_Name;
};

// ============ Player ============
class Player
{
public:
    Player(const std::string& name) : m_Name(name), m_HP(100)
    {
        std::cout << "[Player] " << m_Name << " 생성\n";
    }
    ~Player()
    {
        std::cout << "[Player] " << m_Name << " 사망\n";
    }

    std::string GetName() const { return m_Name; }
    int GetHP() const { return m_HP; }
    void TakeDamage(int dmg) { m_HP -= dmg; }

private:
    std::string m_Name;
    int m_HP;
};

// ============ Monster ============
class Monster
{
public:
    Monster(const std::string& name, std::shared_ptr<Texture> tex)
        : m_Name(name), m_Texture(tex)
    {
        std::cout << "[Monster] " << m_Name << " 스폰\n";
    }
    ~Monster()
    {
        std::cout << "[Monster] " << m_Name << " 사망\n";
    }

    // Player 타겟 설정 (weak_ptr이 shared_ptr을 받아 저장)
    void SetTarget(std::shared_ptr<Player> player)
    {
        m_Target = player;   // shared → weak 자동 변환
    }

    // 타겟 공격 (lock으로 살아있는지 확인)
    void Attack()
    {
        if (auto sp = m_Target.lock())   // 살아있으면 shared_ptr 반환
        {
            std::cout << "[" << m_Name << "] " << sp->GetName() << " 공격! (-10)\n";
            sp->TakeDamage(10);
        }
        else
        {
            std::cout << "[" << m_Name << "] 타겟 없음\n";
        }
    }

private:
    std::string m_Name;
    std::shared_ptr<Texture> m_Texture;   // 텍스처는 여러 몬스터가 공유
    std::weak_ptr<Player> m_Target;       // 플레이어 타겟 (소유 안 함)
};

// ============ GameWorld ============
class GameWorld
{
public:
    GameWorld()
    {
        std::cout << "=== 게임 월드 시작 ===\n";
    }
    ~GameWorld()
    {
        std::cout << "=== 게임 월드 종료 ===\n";
    }

    void Init()
    {
        // 플레이어 생성
        m_Player = std::make_shared<Player>("용사");

        // 텍스처 로드 (몬스터들이 공유)
        auto tex = std::make_shared<Texture>("goblin.png");

        // 몬스터 3마리 생성, 모두 같은 텍스처 사용
        m_Monsters.push_back(std::make_shared<Monster>("Goblin1", tex));
        m_Monsters.push_back(std::make_shared<Monster>("Goblin2", tex));
        m_Monsters.push_back(std::make_shared<Monster>("Goblin3", tex));

        // 모든 몬스터의 타겟을 플레이어로 설정
        for (auto& mob : m_Monsters)
            mob->SetTarget(m_Player);
    }

    void Update()
    {
        for (auto& mob : m_Monsters)
            mob->Attack();

        std::cout << "[Player HP] " << m_Player->GetHP() << "\n";
    }

private:
    std::shared_ptr<Player> m_Player;
    std::vector<std::shared_ptr<Monster>> m_Monsters;
};

// ============ main ============
int main()
{
    GameWorld world;
    world.Init();
    world.Update();
    return 0;
}
